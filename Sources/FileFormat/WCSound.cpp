
#include "Precomp.h"
#include "WCSound.h"
#include "WCArchive.h"
#include "WCPak.h"
#include "FileEntryReader.h"

WCVOCSound::WCVOCSound(const std::string& filename, WCArchive* archive)
{
	FileEntryReader reader = archive->openFile(filename);
	Load(reader);
}

WCVOCSound::WCVOCSound(FileEntryReader reader)
{
	Load(reader);
}

void WCVOCSound::Load(FileEntryReader& reader)
{
	size_t vocstart = reader.Tell();

	char magic[19] = {};
	reader.Read(magic, 19);
	if (memcmp(magic, "Creative Voice File", 19) != 0)
		throw std::runtime_error("Not a creative voice file");
	if (reader.ReadUint8() != 0x1a)
		throw std::runtime_error("Not a creative voice file");

	size_t offset = reader.ReadUint16();
	reader.Seek(vocstart + offset);

	while (reader.Tell() != reader.Size())
	{
		uint8_t type = reader.ReadUint8();
		if (type == 0) // Terminator
		{
			break;
		}
		else if (type == 1) // Sound data
		{
			size_t size = reader.ReadUint24();
			if (size < 2)
				throw std::runtime_error("Invalid creative voice file data header");
			size -= 2;

			if (samplerate != 0)
				throw std::runtime_error("Sound data is changing frequency during playback!");

			int rate = reader.ReadUint8();
			int codec = reader.ReadUint8();

			samplerate = 1000000 / (256 - rate);

			size_t pos = samples.size();
			samples.resize(pos + size);
			reader.Read(samples.data() + pos, size);
		}
		else if (type == 2) // More sound data
		{
			size_t size = reader.ReadUint24();
			size_t pos = samples.size();
			samples.resize(pos + size);
			reader.Read(samples.data() + pos, size);
		}
		else if (type == 3) // Silence
		{
			// size_t size = reader.ReadUint24(); // 3
			// size_t samplecount = reader.ReadUint16();
			// int rate = reader.ReadUint8();
			throw std::runtime_error("Sound data contains silence!");
		}
		else if (type == 4) // Marker
		{
			// size_t size = reader.ReadUint24(); // 2
			// size_t marker = reader.ReadUint16();
			throw std::runtime_error("Sound data contains markers!");
		}
		else if (type == 5) // ASCII string
		{
			size_t size = reader.ReadUint24();
			std::vector<char> buffer(size + 1);
			reader.Read(buffer.data(), size);
			comment += buffer.data();
		}
		else if (type == 6) // Repeat
		{
			// size_t size = reader.ReadUint24(); // 2
			// size_t repetitions = reader.ReadUint16(); // 0xffff = infinite
			throw std::runtime_error("Sound data contains repetitions");
		}
		else if (type == 7) // end repeat
		{
			throw std::runtime_error("Sound data contains repetitions");
		}
		else if (type == 8) // extended
		{
			// size_t size = reader.ReadUint24();
			// reader.Seek(reader.Tell() + size);
			throw std::runtime_error("Sound data contains extensions");
		}
	}

	if (samples.empty())
		throw std::runtime_error("Creative voice file contained no audio samples");
}
