
#include "Precomp.h"
#include "WCImage.h"
#include "WCPalette.h"
#include "FileEntryReader.h"
#include <stdexcept>

WCImage::WCImage(FileEntryReader& reader)
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

		std::vector<uint8_t> image(width * height);
		std::vector<uint8_t> mask(width * height);

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

			uint8_t* dest = &image[(x - minx) + (y - miny) * width];
			uint8_t* destmask = &mask[(x - minx) + (y - miny) * width];

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
						uint8_t color = reader.ReadUint8();
						for (int i = 0; i < runWidth; i++)
						{
							*(dest++) = color;
							*(destmask++) = 255;
						}
					}
					else
					{
						for (int i = 0; i < runWidth; i++)
						{
							uint8_t color = reader.ReadUint8();
							*(dest++) = color;
							*(destmask++) = 255;
						}
					}

					segWidth -= runWidth;
				}
			}
			else
			{
				for (int i = 0; i < segWidth; i++)
				{
					uint8_t color = reader.ReadUint8();
					*(dest++) = color;
					*(destmask++) = 255;
				}
			}
		}

		WCImageFrame frame;
		frame.x = minx;
		frame.y = miny;
		frame.width = width;
		frame.height = height;
		frame.pixels = std::move(image);
		frame.mask = std::move(mask);
		frames.push_back(std::move(frame));
	}

	reader.Seek(baseoffset + sectionsize);
}

std::vector<uint32_t> WCImageFrame::ToBgra8(const WCPalette* palette) const
{
	const uint8_t* src = pixels.data();
	const uint8_t* m = mask.data();
	const uint32_t* pal = palette->palette;
	std::vector<uint32_t> img(pixels.size());
	uint32_t* dest = img.data();
	size_t count = img.size();
	for (size_t i = 0; i < count; i++)
	{
		if (m[i] != 0)
			dest[i] = pal[src[i]];
	}
	return img;
}
