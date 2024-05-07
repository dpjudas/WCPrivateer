#pragma once

#include <string>
#include <cstdint>

class WCArchive;
class FileEntryReader;

class WCPalette
{
public:
	WCPalette();
	WCPalette(const std::string& filename, WCArchive* archive);
	WCPalette(FileEntryReader reader);

	uint32_t palette[256];
	int start = 0;
	int count = 0;

private:
	void ReadPalette(FileEntryReader& reader);
};
