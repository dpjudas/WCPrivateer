
#include "Precomp.h"
#include "WCPak.h"
#include "WCArchive.h"
#include "FileEntryReader.h"

WCPak::WCPak(const std::string& filename, WCArchive& archive)
{
	FileEntryReader reader = archive.openFile(filename);

	std::vector<size_t> offsets;

	uint32_t sectionsize = reader.ReadUint32();
	uint32_t firstoffset = reader.ReadUint24();
	uint8_t unknown = reader.ReadUint8();
	offsets.push_back(firstoffset);

	uint32_t count = (firstoffset - 4) / 4;
	for (uint32_t i = 1; i < count; i++)
	{
		uint32_t offset = reader.ReadUint24();
		uint8_t unknown = reader.ReadUint8();
		if (offset < firstoffset || offset >= sectionsize)
			break;
		offsets.push_back(offset);
	}
	offsets.push_back(sectionsize);

	int filecount = (int)offsets.size() - 1;
	for (int index = 0; index < filecount; index++)
	{
		reader.Seek(offsets[index]);

		std::vector<uint8_t> file(offsets[index + 1] - offsets[index]);
		memcpy(file.data(), reader.CurrentPosData(), file.size());
		files.push_back(std::move(file));
	}
}

FileEntryReader WCPak::openFile(int index)
{
	return FileEntryReader(files[index]);
}
