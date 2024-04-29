#pragma once

#include <string>
#include <vector>
#include <cstdint>

class WCArchive;
class FileEntryReader;

class WCPak
{
public:
	WCPak(const std::string& filename, WCArchive& archive);

	FileEntryReader openFile(int index);

	std::vector<std::vector<uint8_t>> files;
};
