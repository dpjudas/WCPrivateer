
#include "Precomp.h"
#include "WCArchive.h"
#include "FileEntryReader.h"
#include <cstdio>
#include <stdexcept>

WCArchive::WCArchive(const std::string& filename)
{
	f = fopen(filename.c_str(), "rb");
	if (!f)
		throw std::runtime_error("Could not open '" + filename + "'");

	uint32_t count = 0;
	uint32_t dataoffset = 0;
	fread(&count, 4, 1, f);
	fread(&dataoffset, 4, 1, f);

	for (uint32_t i = 0; i < count; i++)
	{
		uint8_t unknown = 0;
		char filename[66] = {};
		uint32_t fileoffset = 0;
		uint32_t filesize = 0;
		fread(&unknown, 1, 1, f);
		fread(filename, 65, 1, f);
		fread(&fileoffset, 4, 1, f);
		fread(&filesize, 4, 1, f);
		if (memcmp(filename, "..\\..\\", 6) == 0)
			memmove(filename, filename + 6, 60);

		FileEntry e;
		e.filename = filename;
		e.offset = fileoffset;
		e.size = filesize;
		entries.push_back(e);

		filenameToIndex[filename] = i;
	}
}

WCArchive::~WCArchive()
{
	fclose(f);
}

FileEntryReader WCArchive::openFile(std::string name)
{
	auto it = filenameToIndex.find(name);
	if (it == filenameToIndex.end())
		throw std::runtime_error("Could not find '" + name + "'");
	return openFile(it->second);
}

FileEntryReader WCArchive::openFile(int index)
{
	auto& e = entries[index];
	fseek(f, e.offset, SEEK_SET);
	std::vector<uint8_t> buffer(e.size);
	if (fread(buffer.data(), buffer.size(), 1, f) <= 0)
		throw std::runtime_error("Could not read '" + e.filename + "'");
	return FileEntryReader(std::move(buffer));
}
