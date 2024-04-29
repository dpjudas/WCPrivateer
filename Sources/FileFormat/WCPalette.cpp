
#include "Precomp.h"
#include "WCPalette.h"
#include "WCArchive.h"
#include "FileEntryReader.h"
#include <stdexcept>

WCPalette::WCPalette(const std::string& filename, WCArchive& archive)
{
	FileEntryReader reader = archive.openFile(filename);

	uint32_t version = reader.ReadUint32BE();
	if (version != 1)
		throw std::runtime_error("Unknown palette version");

	for (int i = 0; i < 256; i++)
	{
		uint32_t red = reader.ReadUint8() << 2;
		uint32_t green = reader.ReadUint8() << 2;
		uint32_t blue = reader.ReadUint8() << 2;
		uint32_t alpha = 255;
		uint32_t rgbacolor = (alpha << 24) | (blue << 16) | (green << 8) | red;
		palette[i] = rgbacolor;
	}
}

WCPalette::WCPalette(FileEntryReader reader)
{
	uint32_t version = reader.ReadUint32BE();
	if (version != 1)
		throw std::runtime_error("Unknown palette version");

	for (int i = 0; i < 256; i++)
	{
		uint32_t red = reader.ReadUint8() << 2;
		uint32_t green = reader.ReadUint8() << 2;
		uint32_t blue = reader.ReadUint8() << 2;
		uint32_t alpha = 255;
		uint32_t rgbacolor = (alpha << 24) | (blue << 16) | (green << 8) | red;
		palette[i] = rgbacolor;
	}
}
