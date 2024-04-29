#pragma once

#include <vector>

class FileEntryReader;
class WCPalette;

class WCImageFrame
{
public:
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	std::vector<uint32_t> pixels;
};

class WCImage
{
public:
	WCImage(FileEntryReader& reader, WCPalette* palette);

	std::vector<WCImageFrame> frames;
};
