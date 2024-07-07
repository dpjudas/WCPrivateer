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
	std::vector<uint8_t> pixels;
	std::vector<uint8_t> mask;

	std::vector<uint32_t> ToBgra8(const WCPalette* palette) const;
};

class WCImage
{
public:
	WCImage(FileEntryReader& reader);

	std::vector<WCImageFrame> frames;
};
