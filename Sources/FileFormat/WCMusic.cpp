
#include "Precomp.h"
#include "WCMusic.h"
#include "WCArchive.h"
#include "FileEntryReader.h"

WCMusic::WCMusic(const std::string& filename, WCArchive* archive)
{
	FileEntryReader reader = archive->openFile(filename);

	std::vector<size_t> offsets;
	if (reader.ReadTag() == "FORM")
	{
		offsets.push_back(0);
	}
	else
	{
		reader.Seek(0);

		size_t baseoffset = reader.Tell();
		uint32_t sectionsize = reader.ReadUint32();
		uint32_t firstoffset = reader.ReadUint24();
		uint8_t unknown = reader.ReadUint8();
		offsets.push_back(baseoffset + firstoffset);

		uint32_t count = (firstoffset - 4) / 4;
		if (count == 0)
			throw std::runtime_error("Invalid music header");

		for (uint32_t i = 1; i < count; i++)
		{
			uint32_t offset = reader.ReadUint24();
			uint8_t unknown = reader.ReadUint8();
			if (offset < firstoffset || offset >= sectionsize)
				break;
			offsets.push_back(baseoffset + offset);
		}
	}
	offsets.push_back(reader.Size());

	int songcount = (int)offsets.size() - 1;
	for (int songindex = 0; songindex < songcount; songindex++)
	{
		reader.Seek(offsets[songindex]);
		std::vector<uint8_t> data(offsets[songindex + 1] - offsets[songindex]);
		reader.Read(data.data(), data.size());
		songs.push_back(std::move(data));
	}
}
