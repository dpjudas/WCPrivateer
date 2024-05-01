
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

		int x2 = reader.ReadInt16();
		int x1 = -reader.ReadInt16();
		int y1 = -reader.ReadInt16();
		int y2 = reader.ReadInt16();

		if (x1 <= -500 || y1 <= -500 || x2 >= 500 || y2 >= 500)
		{
			throw std::runtime_error("Invalid image header");
		}

		std::vector<uint32_t> pixels(1000 * 1000, 0x00000000);

		int minx = 10000;
		int maxx = -10000;
		int miny = 10000;
		int maxy = -10000;
		while (true)
		{
			int count = reader.ReadUint16();
			if (count == 0)
				break;

			int deltax = reader.ReadInt16();
			int deltay = reader.ReadInt16();

			bool isRLE = (count & 1) == 1;
			count >>= 1;

			int x = 500 + deltax;
			int y = 500 + deltay;

			minx = std::min(minx, x);
			maxx = std::max(maxx, x);
			miny = std::min(miny, y);
			maxy = std::max(maxy, y);

			if (isRLE)
			{
				int b = 0;
				while (b < count)
				{
					int count2 = reader.ReadUint8();
					if (count2 == 0)
						goto exitloop;
					bool isSecondaryRLE = (count2 & 1) == 1;
					count2 >>= 1;
					if (isSecondaryRLE)
					{
						uint32_t color = reader.ReadUint8();
						uint32_t rgbacolor = palette->palette[color];
						for (int i = 0; i < count2; i++, x++)
						{
							pixels[x + y * 1000] = rgbacolor;
						}
					}
					else
					{
						for (int a = 0; a < count2; a++, x++)
						{
							uint32_t color = reader.ReadUint8();
							uint32_t rgbacolor = palette->palette[color];
							pixels[x + y * 1000] = rgbacolor;
						}
					}
					b += count2;
					minx = std::min(minx, x);
					maxx = std::max(maxx, x);
				}
			}
			else
			{
				for (int i = 0; i < count; i++, x++)
				{
					uint32_t color = reader.ReadUint8();
					uint32_t rgbacolor = palette->palette[color];
					pixels[x + y * 1000] = rgbacolor;
				}
				minx = std::min(minx, x);
				maxx = std::max(maxx, x);
			}
		}
	exitloop:

		if (minx == 10000 || maxx == -10000 || miny == 10000 || maxy == -10000)
		{
			throw std::runtime_error("Invalid image data");
		}

		int width = maxx - minx;
		int height = maxy - miny + 1;
		std::vector<uint32_t> image(width * height);
		for (int y = miny; y <= maxy; y++)
		{
			int desty = y - miny;
			for (int x = minx; x < maxx; x++)
			{
				int destx = x - minx;
				image[destx + desty * width] = pixels[x + y * 1000];
			}
		}

		WCImageFrame frame;
		frame.x = minx - 500;
		frame.y = miny - 500;
		frame.width = width;
		frame.height = height;
		frame.pixels = std::move(image);
		frames.push_back(std::move(frame));
	}
}
