
#include "Precomp.h"
#include "WCImage.h"
#include "WCPalette.h"
#include "FileEntryReader.h"
#include <stdexcept>

WCImage::WCImage(FileEntryReader& reader, WCPalette* palette)
{
	std::vector<size_t> offsets;

	size_t baseoffset = reader.Tell();
	uint32_t sectionsize = reader.ReadUint32();
	uint32_t firstoffset = reader.ReadUint24();
	uint8_t unknown = reader.ReadUint8();
	offsets.push_back(baseoffset + firstoffset);

	uint32_t count = (firstoffset - 4) / 4;
	if (count == 0)
		throw std::runtime_error("Invalid image header");

	for (uint32_t i = 1; i < count; i++)
	{
		uint32_t offset = reader.ReadUint24();
		uint8_t unknown = reader.ReadUint8();
		if (offset < firstoffset || offset >= sectionsize)
			break;
		offsets.push_back(baseoffset + offset);
	}

	int imagecount = (int)offsets.size();
	for (int imageindex = 0; imageindex < imagecount; imageindex++)
	{
		reader.Seek(offsets[imageindex]);

		int extentsRight = reader.ReadInt16();
		int extentsLeft = reader.ReadInt16();
		int extentsTop = reader.ReadInt16();
		int extentsBottom = reader.ReadInt16();

		int minx = -extentsLeft;
		int miny = -extentsTop;
		int maxx = extentsRight + 1;
		int maxy = extentsBottom + 1;
		int width = maxx - minx;
		int height = maxy - miny;

		if (width <= 0 || height <= 0 || width > 500 || height > 500)
		{
			throw std::runtime_error("Invalid image header");
		}

		std::vector<uint32_t> image(width * height);

		while (true)
		{
			int segWidth = reader.ReadUint16();
			if (segWidth == 0)
				break;
			bool segIsRun = (segWidth & 1) != 0;
			segWidth >>= 1;

			int x = reader.ReadInt16();
			int y = reader.ReadInt16();

			if (x < minx || x + segWidth > maxx || y < miny || y >= maxy)
				throw std::runtime_error("Invalid image data"); // Segment out of bounds

			uint32_t* dest = &image[(x - minx) + (y - miny) * width];

			if (segIsRun)
			{
				while (segWidth > 0)
				{
					int runWidth = reader.ReadUint8();
					bool runIsRLE = (runWidth & 1) != 0;
					runWidth >>= 1;

					if (runWidth > segWidth)
						throw std::runtime_error("Invalid image data"); // Run out of bounds

					if (runIsRLE)
					{
						uint32_t color = reader.ReadUint8();
						uint32_t rgbacolor = palette->palette[color];
						for (int i = 0; i < runWidth; i++)
						{
							*(dest++) = rgbacolor;
						}
					}
					else
					{
						for (int i = 0; i < runWidth; i++)
						{
							uint32_t color = reader.ReadUint8();
							uint32_t rgbacolor = palette->palette[color];
							*(dest++) = rgbacolor;
						}
					}

					segWidth -= runWidth;
				}
			}
			else
			{
				for (int i = 0; i < segWidth; i++)
				{
					uint32_t color = reader.ReadUint8();
					uint32_t rgbacolor = palette->palette[color];
					*(dest++) = rgbacolor;
				}
			}
		}

		WCImageFrame frame;
		frame.x = minx;
		frame.y = miny;
		frame.width = width;
		frame.height = height;
		frame.pixels = std::move(image);
		frames.push_back(std::move(frame));
	}
}
