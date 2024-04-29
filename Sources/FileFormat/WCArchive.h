#pragma once

#include <vector>
#include <map>
#include <string>
#include <cstdint>

class FileEntryReader;

class WCArchive
{
public:
	WCArchive(const std::string& filename);
	~WCArchive();

	FileEntryReader openFile(std::string name);
	FileEntryReader openFile(int index);

	const std::string& getFilename(int index) const { return entries[index].filename; }
	int getFileCount() const { return (int)entries.size(); }

private:
	struct FileEntry
	{
		std::string filename;
		uint32_t offset = 0;
		uint32_t size = 0;
	};
	std::vector<FileEntry> entries;
	std::map<std::string, int> filenameToIndex;
	FILE* f = nullptr;

	WCArchive(const WCArchive&) = delete;
	WCArchive& operator=(const WCArchive&) = delete;
};
