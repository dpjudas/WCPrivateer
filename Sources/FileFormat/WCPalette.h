#pragma once

#include <string>
#include <cstdint>

class WCArchive;
class FileEntryReader;

class WCPalette
{
public:
	WCPalette(const std::string& filename, WCArchive& archive);
	WCPalette(FileEntryReader reader);

	uint32_t palette[256];
};
