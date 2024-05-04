#pragma once

class WCArchive;
class FileEntryReader;

class WCMusic
{
public:
	WCMusic(const std::string& filename, WCArchive* archive);

	std::vector<std::vector<uint8_t>> songs;
};
