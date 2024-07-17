
#pragma once

#include <memory>
#include <vector>
#include <cstdint>
#include <zmusic.h>

class AudioSource
{
public:
	static ZMusic_MusicStream OpenSong(std::vector<uint8_t> filedata);
	static std::unique_ptr<AudioSource> CreateZMusic(ZMusic_MusicStream song);
	static std::unique_ptr<AudioSource> CreateResampler(int targetFrequency, std::unique_ptr<AudioSource> source);
	static std::unique_ptr<AudioSource> CreateSample(int frequency, const uint8_t* data, size_t size);

	virtual ~AudioSource() = default;
	virtual int GetFrequency() = 0;
	virtual int GetChannels() = 0;
	virtual int GetSamples() = 0;
	virtual void SeekToSample(uint64_t position) = 0;
	virtual size_t ReadSamples(float* output, size_t samples) = 0;
};
