#pragma once

class WCArchive;
class FileEntryReader;

class WCVOCSound
{
public:
	WCVOCSound(const std::string& filename, WCArchive* archive);
	WCVOCSound(FileEntryReader reader);

	int samplerate = 0;
	std::vector<uint8_t> samples;
	std::string comment;

private:
	void Load(FileEntryReader& reader);
};
