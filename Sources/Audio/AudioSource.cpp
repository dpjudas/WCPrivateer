
#include "Precomp.h"
#include "AudioSource.h"
#include "Util/UTF16.h"
#include "FileFormat/FileEntryReader.h"
#include "resample/CDSPResampler.h"
#include <stdexcept>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

class ZMusicAudioSource : public AudioSource
{
public:
	ZMusicAudioSource(ZMusic_MusicStream song) : song(song)
	{
		// Hmm, doesn't exist in the lib file!
		// ZMusic_GetStreamInfoEx(song, &info);
		SoundStreamInfo oldinfo = {};
		ZMusic_GetStreamInfo(song, &oldinfo);
		info.mBufferSize = oldinfo.mBufferSize;
		info.mSampleRate = oldinfo.mSampleRate;
		info.mChannelConfig = (oldinfo.mNumChannels == -2 || oldinfo.mNumChannels == 2) ? ChannelConfig_Stereo : ChannelConfig_Mono;
		info.mSampleType = oldinfo.mNumChannels < 0 ? SampleType_Int16 : SampleType_Float32;

		for (int i = 0; i < 3; i++)
			fillbuffers.push_back(std::vector<uint8_t>((size_t)info.mBufferSize));

		size = info.mBufferSize / GetBytesPerSample();
		pos = size;

		thread = std::thread([this]() { WorkerThreadMain(); });
	}

	~ZMusicAudioSource()
	{
		std::unique_lock lock(mutex);
		exitflag = true;
		lock.unlock();
		condvar.notify_all();
		thread.join();
	}

	int GetFrequency() override
	{
		return info.mSampleRate;
	}

	int GetChannels() override
	{
		return info.mChannelConfig == ChannelConfig_Stereo ? 2 : 1;
	}

	int GetSamples() override { return 0; }
	void SeekToSample(uint64_t position) override { }

	size_t ReadSamples(float* output, size_t samples) override
	{
		size_t pos = 0;
		while (pos < samples)
		{
			size_t available = ReadSamplesImpl(output + pos, samples - pos);
			if (available == 0)
				break;
			pos += available;
		}
		return pos;
	}

	size_t ReadSamplesImpl(float* output, size_t samples)
	{
		size_t available = size - pos;
		if (available == 0)
		{
			GetBuffer();
			available = size - pos;
		}

		size_t count = std::min(samples, available);
		if (info.mSampleType == SampleType_UInt8)
		{
			uint8_t* src = reinterpret_cast<uint8_t*>(buffer.data()) + pos;
			for (size_t i = 0; i < count; i++)
			{
				*(output++) = *(src++) * (1.0f / 255.0f);
			}
		}
		else if (info.mSampleType == SampleType_Int16)
		{
			int16_t* src = reinterpret_cast<int16_t*>(buffer.data()) + pos;
			for (size_t i = 0; i < count; i++)
			{
				*(output++) = *(src++) * (1.0f / 32768.0f);
			}
		}
		else if (info.mSampleType == SampleType_Float32)
		{
			float* src = reinterpret_cast<float*>(buffer.data()) + pos;
			for (size_t i = 0; i < count; i++)
			{
				*(output++) = *(src++) * 1.0f;
			}
		}
		pos += count;
		return count;
	}

	void GetBuffer()
	{
		std::unique_lock lock(mutex);
		if (!buffer.empty())
		{
			fillbuffers.push_back(std::move(buffer));
			buffer = {};
		}
		if (!playbuffers.empty())
		{
			buffer = std::move(playbuffers.front());
			playbuffers.erase(playbuffers.begin());
			pos = 0;
		}
		lock.unlock();
		condvar.notify_all();
	}

	int GetBytesPerSample()
	{
		if (info.mSampleType == SampleType_UInt8)
		{
			return 1;
		}
		else if (info.mSampleType == SampleType_Int16)
		{
			return 2;
		}
		else if (info.mSampleType == SampleType_Float32)
		{
			return 4;
		}
		else
		{
			throw std::runtime_error("Unknown sample type");
		}
	}

	void WorkerThreadMain()
	{
		std::unique_lock lock(mutex);
		while (true)
		{
			condvar.wait(lock, [&]() { return !fillbuffers.empty() || exitflag; });
			if (exitflag)
				break;

			std::vector<uint8_t> buffer = std::move(fillbuffers.back());
			fillbuffers.pop_back();
			lock.unlock();

			if (!ZMusic_FillStream(song, buffer.data(), buffer.size()))
			{
				memset(buffer.data(), 0, buffer.size());
			}

			lock.lock();
			playbuffers.push_back(std::move(buffer));
		}
	}

	ZMusic_MusicStream song = nullptr;
	SoundStreamInfoEx info = {};
	std::vector<uint8_t> buffer;
	std::vector<std::vector<uint8_t>> playbuffers;
	std::vector<std::vector<uint8_t>> fillbuffers;
	bool exitflag = false;
	size_t pos = 0;
	size_t size = 0;
	std::thread thread;
	std::mutex mutex;
	std::condition_variable condvar;
};

std::unique_ptr<AudioSource> AudioSource::CreateZMusic(ZMusic_MusicStream song)
{
	return std::make_unique<ZMusicAudioSource>(song);
}

class ZMusicInit
{
public:
	ZMusicInit()
	{
		ZMusicCallbacks callbacks{};
		callbacks.MessageFunc = [](int severity, const char* msg)
			{
			};
		callbacks.NicePath = [](const char* path) -> const char*
			{
				return path;
			};
		callbacks.PathForSoundfont = [](const char* name, int type) -> const char*
			{
				std::vector<wchar_t> exepath16(1024);
				GetModuleFileName(0, exepath16.data(), 1023);
				std::string exepath = from_utf16(exepath16.data());
				exepath = exepath.substr(0, exepath.find_last_of("\\/"));
				static std::string horribleAPI;
				horribleAPI = exepath + "/soundfonts/WCPrivateer.sf2";
				return horribleAPI.c_str();
			};
		callbacks.OpenSoundFont = [](const char* name, int type) -> void*
			{
				return nullptr;
			};
		callbacks.SF_OpenFile = [](void* handle, const char* filename) -> ZMusicCustomReader* {
			FILE* file = fopen(filename, "rb");
			if (!file)
				return nullptr;

			ZMusicCustomReader* zcr = new ZMusicCustomReader;
			zcr->handle = file;
			zcr->gets = [](ZMusicCustomReader* zr, char* buff, int n) -> char*
				{
					auto f = reinterpret_cast<FILE*>(zr->handle);
					auto stringstart = ftell(f);
					int pos = 0;
					while (pos < n)
					{
						size_t c = fread(buff + pos, 1, 1, f);
						if (c != 1)
							break;
						if (buff[pos] == 0)
							return buff;
						if (buff[pos] != '\r')
							pos++;
					}
					fseek(f, stringstart, SEEK_SET);
					return nullptr;
				};
			zcr->read = [](ZMusicCustomReader* zr, void* buff, int32_t size) -> long
				{
					return (long)fread(buff, 1, size, reinterpret_cast<FILE*>(zr->handle));
				};
			zcr->seek = [](ZMusicCustomReader* zr, long offset, int whence) -> long
				{
					return (long)fseek(reinterpret_cast<FILE*>(zr->handle), offset, whence);
				};

			zcr->tell = [](ZMusicCustomReader* zr) -> long
				{
					return (long)ftell(reinterpret_cast<FILE*>(zr->handle));
				};

			zcr->close = [](ZMusicCustomReader* zr)
				{
					fclose(reinterpret_cast<FILE*>(zr->handle));
					delete zr;
				};
			return zcr;
			};
		callbacks.SF_AddToSearchPath = [](void* handle, const char* path) {};
		callbacks.SF_Close = [](void* handle) {};

		ZMusic_SetCallbacks(&callbacks);
	}
};

ZMusic_MusicStream AudioSource::OpenSong(std::vector<uint8_t> filedata)
{
	static ZMusicInit init;

	FileEntryReader* reader = new FileEntryReader(std::move(filedata));

	ZMusicCustomReader* zcr = new ZMusicCustomReader;
	zcr->handle = reader;
	zcr->gets = [](ZMusicCustomReader* zr, char* buff, int n) -> char*
		{
			auto f = reinterpret_cast<FileEntryReader*>(zr->handle);
			auto stringstart = f->Tell();
			int pos = 0;
			while (pos < n)
			{
				size_t c = f->ReadUint8();
				if (c != 1)
					break;
				if (buff[pos] == 0)
					return buff;
				if (buff[pos] != '\r')
					pos++;
			}
			f->Seek(stringstart);
			return nullptr;
		};
	zcr->read = [](ZMusicCustomReader* zr, void* buff, int32_t size) -> long
		{
			return (long)reinterpret_cast<FileEntryReader*>(zr->handle)->Read(buff, size);
		};
	zcr->seek = [](ZMusicCustomReader* zr, long offset, int whence) -> long
		{
			auto reader = reinterpret_cast<FileEntryReader*>(zr->handle);
			switch (whence)
			{
			default:
			case SEEK_SET:
				reader->Seek(offset);
				break;
			case SEEK_CUR:
				reader->Seek((int64_t)reader->Tell() + offset);
				break;
			case SEEK_END:
				reader->Seek((int64_t)reader->Size() + offset);
				break;
			}
			return 0;
		};

	zcr->tell = [](ZMusicCustomReader* zr) -> long
		{
			return (long)reinterpret_cast<FileEntryReader*>(zr->handle)->Tell();
		};

	zcr->close = [](ZMusicCustomReader* zr)
		{
			delete reinterpret_cast<FileEntryReader*>(zr->handle);
			delete zr;
		};

	return ZMusic_OpenSong(zcr, MDEV_FLUIDSYNTH, "");
}

class ResampleAudioSource : public AudioSource
{
public:
	ResampleAudioSource(int targetFrequency, std::unique_ptr<AudioSource> audiosource) : targetFrequency(targetFrequency), source(std::move(audiosource))
	{
		channels.resize(source->GetChannels());
		for (auto& channel : channels)
		{
			channel.resampler = std::make_unique<r8b::CDSPResampler24>(source->GetFrequency(), targetFrequency, maxInSize);
			channel.inputbuffer.resize(maxInSize);
		}
		srcbuffer.resize(maxInSize * channels.size());
	}

	int GetFrequency()
	{
		return targetFrequency;
	}

	int GetChannels()
	{
		return source->GetChannels();
	}

	int GetSamples()
	{
		return (int)(source->GetSamples() * (int64_t)targetFrequency / source->GetFrequency());
	}

	void SeekToSample(uint64_t position)
	{
		source->SeekToSample(position * source->GetFrequency() / targetFrequency);
		for (auto& channel : channels)
			channel.resampler->clear();
	}

	size_t ReadSamples(float* output, size_t samples)
	{
		size_t samplesrequested = samples;
		while (samples > 0)
		{
			ProcessInput();

			size_t channelcount = channels.size();
			size_t count = GetOutputCount(samples / channelcount);
			if (count == 0)
				break;

			for (size_t i = 0; i < count; i++)
			{
				for (size_t c = 0; c < channelcount; c++)
				{
					*(output++) = (float)channels[c].outputbuffer[i];
				}
			}

			for (size_t c = 0; c < channelcount; c++)
			{
				channels[c].outputbuffer += count;
				channels[c].outputavailable -= count;
			}
			samples -= count * channels.size();
		}

		return samplesrequested - samples;
	}

	void ProcessInput()
	{
		size_t needed = srcbuffer.size();
		for (Channel& channel : channels)
			needed = std::min(needed, channel.inputbuffer.size() - channel.inputavailable);

		if (needed > 0 && !endofdata)
		{
			size_t available = source->ReadSamples(srcbuffer.data(), needed);
			endofdata = (available == 0);
			size_t channelcount = channels.size();
			auto src = srcbuffer.data();
			for (size_t c = 0; c < channelcount; c++)
			{
				double* inputbuffer = channels[c].inputbuffer.data();
				int pos = channels[c].inputavailable;
				for (size_t i = c; i < available; i += channelcount)
					inputbuffer[pos++] = src[i];
				channels[c].inputavailable = pos;
			}
		}

		for (Channel& channel : channels)
		{
			if (channel.outputavailable == 0 && channel.inputavailable > 0)
			{
				channel.outputavailable = channel.resampler->process(channel.inputbuffer.data(), channel.inputavailable, channel.outputbuffer);
				channel.inputavailable = 0;
			}
		}
	}

	size_t GetOutputCount(size_t samples)
	{
		size_t count = samples;
		for (auto& channel : channels)
			count = std::min((size_t)channel.outputavailable, count);
		return count;
	}

	struct Channel
	{
		std::vector<double> inputbuffer;
		int inputavailable = 0;
		double* outputbuffer = nullptr;
		int outputavailable = 0;
		std::unique_ptr<r8b::CDSPResampler> resampler;
	};

	std::vector<float> srcbuffer;
	std::vector<Channel> channels;
	int maxInSize = 4096;
	bool endofdata = false;

	int targetFrequency = 0;
	std::unique_ptr<AudioSource> source;
};

std::unique_ptr<AudioSource> AudioSource::CreateResampler(int targetFrequency, std::unique_ptr<AudioSource> source)
{
	return std::make_unique<ResampleAudioSource>(targetFrequency, std::move(source));
}

class SampleAudioSource : public AudioSource
{
public:
	SampleAudioSource(int frequency, const uint8_t* data, size_t size) : frequency(frequency), data(data, data + size)
	{
	}

	int GetFrequency() override
	{
		return frequency;
	}

	int GetChannels() override
	{
		return 1;
	}

	int GetSamples() override
	{
		return data.size();
	}

	void SeekToSample(uint64_t position) override
	{
		pos = position;
	}

	size_t ReadSamples(float* output, size_t samples) override
	{
		size_t available = std::min(samples, data.size() - pos);
		const uint8_t* input = data.data() + pos;
		for (size_t i = 0; i < available; i++)
		{
			output[i] = (input[i] - 128) * (1.0f / 128.0f);
		}
		pos += available;

		if (available < samples)
		{
			for (size_t i = available; i < samples; i++)
				output[i] = 0.0f;
		}

		return samples;
	}

	uint64_t pos = 0;
	int frequency = 0;
	std::vector<uint8_t> data;
};

std::unique_ptr<AudioSource> AudioSource::CreateSample(int frequency, const uint8_t* data, size_t size)
{
	return std::make_unique<SampleAudioSource>(frequency, data, size);
}
