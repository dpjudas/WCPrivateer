
#include "Precomp.h"
#include "WCPalette.h"
#include "WCArchive.h"
#include "FileEntryReader.h"
#include <stdexcept>

WCPalette::WCPalette()
{
	for (int i = 0; i < 256; i++)
	{
		palette[i] = 0;
	}
}

WCPalette::WCPalette(const std::string& filename, WCArchive* archive)
{
	FileEntryReader reader = archive->openFile(filename);
	ReadPalette(reader);
}

WCPalette::WCPalette(FileEntryReader reader)
{
	ReadPalette(reader);
}

void WCPalette::ReadPalette(FileEntryReader& reader)
{
	start = reader.ReadUint16();
	count = reader.ReadUint16();
	if (start + count > 256)
		throw std::runtime_error("Invalid palette");

	for (int i = 0; i < start; i++)
	{
		palette[i] = 0;
	}

	for (int i = start, end = start + count; i < end; i++)
	{
		uint32_t red = reader.ReadUint8() << 2;
		uint32_t green = reader.ReadUint8() << 2;
		uint32_t blue = reader.ReadUint8() << 2;
		uint32_t alpha = 255;
		uint32_t rgbacolor = (alpha << 24) | (blue << 16) | (green << 8) | red;
		palette[i] = rgbacolor;
	}

	for (int i = start + count; i < 256; i++)
	{
		palette[i] = 0;
	}
}
