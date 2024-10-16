
#include "GameScreen.h"
#include "Game/GameApp.h"
#include "FileFormat/WCPak.h"
#include "FileFormat/WCPalette.h"
#include "FileFormat/WCImage.h"
#include "FileFormat/FileEntryReader.h"

GameScreen::GameScreen(GameApp* app) : app(app)
{
}

void GameScreen::PlayMusic(std::string filename, int song)
{
	app->PlayMusic(filename, song);
}

void GameScreen::PlaySound(WCVOCSound* sound)
{
	app->PlaySound(sound);
}

void GameScreen::ShowScreen(std::unique_ptr<GameScreen> screen)
{
	app->ShowScreen(std::move(screen));
}

void GameScreen::PushScreen(std::unique_ptr<GameScreen> screen)
{
	app->PushScreen(std::move(screen));
}

void GameScreen::PopScreen()
{
	app->PopScreen();
}

std::unique_ptr<WCPalette> GameScreen::LoadPakPalette(const std::string& pakFilename, int pakindex)
{
	WCPak palpak(pakFilename, app->archive.get());
	FileEntryReader reader = palpak.openFile(pakindex);
	return std::make_unique<WCPalette>(reader);
}

std::unique_ptr<WCPalette> GameScreen::LoadSpacePalette()
{
	return LoadPalette("DATA\\PALETTE\\SPACE.PAL");
}

std::unique_ptr<WCPalette> GameScreen::LoadPCMainPalette()
{
	return LoadPalette("DATA\\PALETTE\\PCMAIN.PAL");
}

std::unique_ptr<WCPalette> GameScreen::LoadPrefMainPalette()
{
	return LoadPalette("DATA\\PALETTE\\PREFMAIN.PAL");
}

std::unique_ptr<WCPalette> GameScreen::LoadJoyCalibPalette()
{
	return LoadPalette("DATA\\OPTIONS\\JOYCALIB.PAL");
}

std::unique_ptr<WCPalette> GameScreen::LoadPalette(const std::string& filename)
{
	return std::make_unique<WCPalette>(filename, app->archive.get());
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadFontsFont(WCPalette* palette, bool forceWhite)
{
	FileEntryReader reader = app->archive->openFile("DATA\\OPTIONS\\FONTS.IFF");
	reader.PushChunk("FORM");
	reader.ReadTag("FILE");
	reader.PushChunk("FONT");
	reader.ReadTag("conv");
	reader.ReadUint32(); // Was this supposed to be a chunk size? its all zeros
	WCImage image(reader);
	reader.PopChunk(false);
	reader.PopChunk(false);
	return LoadWCImage(image, palette, forceWhite);
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadConvFont(WCPalette* palette)
{
	return LoadShpImage("DATA\\FONTS\\CONVFONT.SHP", palette);
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadDemoFont(WCPalette* palette)
{
	return LoadShpImage("DATA\\FONTS\\DEMOFONT.SHP", palette);
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadMssgFont(WCPalette* palette, int color)
{
	FileEntryReader reader = app->archive->openFile("DATA\\FONTS\\MSSGFONT.SHP");
	WCImage image(reader);
	if (color != -1)
	{
		for (auto& frame : image.frames)
		{
			for (size_t i = 0, count = frame.pixels.size(); i < count; i++)
			{
				if (frame.mask[i] != 0)
					frame.pixels[i] = color;
			}
		}
	}
	return LoadWCImage(image, palette);
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadPCFont(WCPalette* palette)
{
	return LoadShpImage("DATA\\FONTS\\PCFONT.SHP", palette);
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadPrivFont(WCPalette* palette)
{
	return LoadShpImage("DATA\\FONTS\\PRIVFNT.SHP", palette);
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadOptFont(WCPalette* palette)
{
	return LoadShpImage("DATA\\FONTS\\OPTFONT.SHP", palette);
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadDotImage(int red, int green, int blue, int alpha)
{
	std::vector<std::unique_ptr<GameTexture>> frames;
	auto gameTexture = std::make_unique<GameTexture>();
	gameTexture->x = 0;
	gameTexture->y = 0;
	gameTexture->width = 1;
	gameTexture->height = 1;
	gameTexture->pixels.resize(1);
	gameTexture->pixels.front() = (alpha << 24) | (blue << 16) | (green << 8) | red;
	frames.push_back(std::move(gameTexture));
	return frames;
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadNavCircleImage(int red, int green, int blue, int alpha)
{
	uint32_t color = (alpha << 24) | (blue << 16) | (green << 8) | red;
	std::vector<std::unique_ptr<GameTexture>> frames;
	auto gameTexture = std::make_unique<GameTexture>();
	gameTexture->x = -3;
	gameTexture->y = -3;
	gameTexture->width = 7;
	gameTexture->height = 7;
	gameTexture->pixels.resize(7 * 7);
	for (int index : { 2, 3, 4, 8, 12, 14, 20, 21, 27, 28, 34, 36, 40, 44, 45, 46 })
		gameTexture->pixels[index] = color;
	frames.push_back(std::move(gameTexture));
	return frames;
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadNavSquareImage(int red, int green, int blue, int alpha)
{
	uint32_t color = (alpha << 24) | (blue << 16) | (green << 8) | red;
	std::vector<std::unique_ptr<GameTexture>> frames;
	auto gameTexture = std::make_unique<GameTexture>();
	gameTexture->x = -3;
	gameTexture->y = -3;
	gameTexture->width = 7;
	gameTexture->height = 7;
	gameTexture->pixels.resize(7 * 7);
	for (int x = 0; x < 7; x++)
	{
		gameTexture->pixels[x] = color;
		gameTexture->pixels[6 * 7 + x] = color;
	}
	for (int y = 1; y < 6; y++)
	{
		gameTexture->pixels[y * 7] = color;
		gameTexture->pixels[y * 7 + 6] = color;
	}
	frames.push_back(std::move(gameTexture));
	return frames;
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadWCImage(const WCImage& image, const WCPalette* palette, bool forceWhite)
{
	std::vector<std::unique_ptr<GameTexture>> frames;
	for (auto& frame : image.frames)
	{
		auto gameTexture = std::make_unique<GameTexture>();
		gameTexture->x = frame.x;
		gameTexture->y = frame.y;
		gameTexture->width = frame.width;
		gameTexture->height = frame.height;
		gameTexture->pixels = frame.ToBgra8(palette);
		if (forceWhite)
		{
			for (uint32_t& p : gameTexture->pixels)
			{
				if (p & 0xff000000)
					p = 0xffffffff;
			}
		}
		frames.push_back(std::move(gameTexture));
	}
	return frames;
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadPakImage(const std::string& pakFilename, int pakindex, const WCPalette* palette)
{
	WCPak pak(pakFilename, app->archive.get());
	FileEntryReader reader = pak.openFile(pakindex);
	WCImage image(reader);
	return LoadWCImage(image, palette);
}

std::vector<std::unique_ptr<GameTexture>> GameScreen::LoadShpImage(const std::string& filename, const WCPalette* palette, bool forceWhite)
{
	FileEntryReader reader = app->archive->openFile(filename);
	WCImage image(reader);
	return LoadWCImage(image, palette, forceWhite);
}

std::unique_ptr<GameTexture> GameScreen::LoadIffImage(const std::string& filename, int index, const WCPalette* palette)
{
	FileEntryReader reader = app->archive->openFile(filename);

	if (reader.ReadTag() != "FORM")
	{
		throw std::runtime_error("Unexpected IFF contents");
	}
	reader.ReadUint32BE(); // chunk size

	std::string tag = reader.ReadTag();
	if (tag == "APPR")
	{
		tag = reader.ReadTag();
		if (tag != "FORM")
			throw std::runtime_error("Unexpected IFF contents");
		reader.ReadUint32BE(); // chunk size

		tag = reader.ReadTag();
		if (tag == "BMAP")
		{
			tag = reader.ReadTag();
			if (tag != "INFO")
				throw std::runtime_error("Unexpected IFF contents");
			if (reader.ReadUint32BE() != 6) // chunk size, expect 6
				throw std::runtime_error("Unexpected IFF contents");

			uint8_t unknown0 = reader.ReadUint8();
			uint8_t unknown1 = reader.ReadUint8();
			uint8_t unknown2 = reader.ReadUint8();
			uint8_t unknown3 = reader.ReadUint8();
			uint8_t unknown4 = reader.ReadUint8();
			uint8_t unknown5 = reader.ReadUint8();
		}
		else if (tag == "BM3D")
		{
			tag = reader.ReadTag();
			if (tag != "INFO")
				throw std::runtime_error("Unexpected IFF contents");
			if (reader.ReadUint32BE() != 12) // chunk size, expect 12
				throw std::runtime_error("Unexpected IFF contents");

			uint8_t unknown0 = reader.ReadUint8();
			uint8_t unknown1 = reader.ReadUint8();
			uint8_t unknown2 = reader.ReadUint8();
			uint8_t unknown3 = reader.ReadUint8();
			uint8_t unknown4 = reader.ReadUint8();
			uint8_t unknown5 = reader.ReadUint8();
			uint8_t unknown6 = reader.ReadUint8();
			uint8_t unknown7 = reader.ReadUint8();
			uint8_t unknown8 = reader.ReadUint8();
			uint8_t unknown9 = reader.ReadUint8();
			uint8_t unknown10 = reader.ReadUint8();
			uint8_t unknown11 = reader.ReadUint8();
		}
		else
		{
			throw std::runtime_error("Unexpected IFF contents");
		}
	}
	else if (tag == "COCK")
	{
		tag = reader.ReadTag();
		if (tag != "FORM")
			throw std::runtime_error("Unexpected IFF contents");
		reader.ReadUint32BE(); // chunk size

		tag = reader.ReadTag();
		if (tag != "FRNT") // Other sides appear after first one?
			throw std::runtime_error("Unexpected IFF contents");
	}
	else if (tag == "MISC")
	{
		tag = reader.ReadTag();
		if (tag != "TABL")
			throw std::runtime_error("Unexpected IFF contents");
		reader.ReadUint32BE(); // chunk size, is 4 and its an invalid size!

		uint8_t unknown0 = reader.ReadUint8();
		uint8_t unknown1 = reader.ReadUint8();
		uint8_t unknown2 = reader.ReadUint8();
		uint8_t unknown3 = reader.ReadUint8();
		uint8_t unknown4 = reader.ReadUint8();
		uint8_t unknown5 = reader.ReadUint8();
		uint8_t unknown6 = reader.ReadUint8();
		uint8_t unknown7 = reader.ReadUint8();

		tag = reader.ReadTag();
		if (tag != "FORM")
			throw std::runtime_error("Unexpected IFF contents");
		reader.ReadUint32BE(); // chunk size

		tag = reader.ReadTag();
		if (tag != "EXPL")
			throw std::runtime_error("Unexpected IFF contents");

		tag = reader.ReadTag();
		if (tag != "INFO")
			throw std::runtime_error("Unexpected IFF contents");
		if (reader.ReadUint32BE() != 8) // chunk size, expect 8
			throw std::runtime_error("Unexpected IFF contents");

		uint8_t unknown8 = reader.ReadUint8();
		uint8_t unknown9 = reader.ReadUint8();
		uint8_t unknown10 = reader.ReadUint8();
		uint8_t unknown11 = reader.ReadUint8();
		uint8_t unknown12 = reader.ReadUint8();
		uint8_t unknown13 = reader.ReadUint8();
		uint8_t unknown14 = reader.ReadUint8();
		uint8_t unknown15 = reader.ReadUint8();
	}
	else if (tag == "FACE") // DATA\AIDS\MFDFACES.IFF
	{
		tag = reader.ReadTag();
		if (tag != "TABL")
			throw std::runtime_error("Unexpected IFF contents");
		uint32_t chunksize = reader.ReadUint32BE();
		reader.Seek(reader.Tell() + chunksize);

		size_t baseindex = 0;
		while (reader.Tell() < reader.Size())
		{
			uint32_t facesize = reader.ReadUint32();
			size_t faceoffset = reader.Tell();
			if (faceoffset + facesize > reader.Size())
				break;

			tag = reader.ReadTag();
			if (tag != "INFO")
				break;
			if (reader.ReadUint32BE() != 2) // chunk size, expect 2
				break;

			uint8_t unknown0 = reader.ReadUint8();
			uint8_t unknown1 = reader.ReadUint8();

			tag = reader.ReadTag();
			uint32_t chunksize = reader.ReadUint32BE(); // chunk size
			if (tag != "SHAP")
				break;

			WCImage image(reader);
			if (image.frames.size() + baseindex < index)
			{
				auto& frame = image.frames[index - baseindex];
				auto gameTexture = std::make_unique<GameTexture>();
				gameTexture->x = frame.x;
				gameTexture->y = frame.y;
				gameTexture->width = frame.width;
				gameTexture->height = frame.height;
				gameTexture->pixels = frame.ToBgra8(palette);
				return gameTexture;
			}

			reader.Seek(faceoffset + facesize);
		}
		throw std::runtime_error("Unexpected IFF contents");
	}
	else
	{
		throw std::runtime_error("Unexpected IFF contents");
	}

	tag = reader.ReadTag();
	uint32_t chunksize = reader.ReadUint32BE(); // chunk size

	if (tag == "SHAP")
	{
		WCImage image(reader);
		auto& frame = image.frames[index];
		auto gameTexture = std::make_unique<GameTexture>();
		gameTexture->x = frame.x;
		gameTexture->y = frame.y;
		gameTexture->width = frame.width;
		gameTexture->height = frame.height;
		gameTexture->pixels = frame.ToBgra8(palette);
		return gameTexture;
	}
	else if (tag == "VSHP")
	{
		size_t chunkoffset = reader.Tell();

		uint32_t sectionoffset = 0;
		size_t baseindex = 0;
		do
		{
			reader.Seek(chunkoffset + sectionoffset);
			uint32_t sectionsize = reader.ReadUint32();

			reader.Seek(chunkoffset + sectionoffset);
			WCImage image(reader);

			if (image.frames.size() + baseindex < index)
			{
				auto& frame = image.frames[index - baseindex];
				auto gameTexture = std::make_unique<GameTexture>();
				gameTexture->x = frame.x;
				gameTexture->y = frame.y;
				gameTexture->width = frame.width;
				gameTexture->height = frame.height;
				gameTexture->pixels = frame.ToBgra8(palette);
				return gameTexture;
			}
			baseindex += image.frames.size();

			sectionoffset += sectionsize;
		} while (sectionoffset + 4 <= chunksize);
	}

	throw std::runtime_error("No IFF image found");
}

int GameScreen::GetTextWidth(const std::string_view& text, std::vector<std::unique_ptr<GameTexture>>& font, int spacing)
{
	int textwidth = 0;
	int linewidth = 0;
	for (char c : text)
	{
		uint8_t i = c;
		if (i == '\n')
		{
			textwidth = std::max(textwidth, linewidth);
			linewidth = 0;
		}
		else if (i == 32 && font.size() > 'x')
		{
			linewidth += font['x']->width + spacing;
		}
		else if (i < font.size() && !font[i]->pixels.empty())
		{
			linewidth += font[i]->width - font[i]->x + spacing;
		}
	}
	textwidth = std::max(textwidth, linewidth);
	return textwidth;
}

std::string GameScreen::WordWrap(std::string text, int width, std::vector<std::unique_ptr<GameTexture>>& font, int spacing)
{
	std::string result;
	std::string line;
	int x = 0;
	for (char c : text)
	{
		uint8_t i = c;
		if (i == '\n')
		{
			x = 0;
			result += line;
			result.push_back('\n');
			line.clear();
		}
		else if (i == 32 && font.size() > 'x')
		{
			x += font['x']->width + spacing;
			line.push_back(' ');
		}
		else if (i < font.size() && !font[i]->pixels.empty())
		{
			x += font[i]->width - font[i]->x + spacing;
			if (x > width)
			{
				x = 0;
				size_t wordend = line.rfind(' ');
				if (wordend == 0 || wordend == std::string::npos)
				{
					result += line;
					result.push_back('\n');
					line.clear();
				}
				else
				{
					result += line.substr(0, wordend);
					while (!result.empty() && result.back() == ' ')
						result.pop_back();
					result.push_back('\n');
					line = line.substr(wordend + 1);
				}
				line.push_back(c);
				x = GetTextWidth(line, font, spacing);
			}
			else
			{
				line.push_back(c);
			}

		}
	}
	result += line;
	return result;
}

void GameScreen::DrawText(RenderDevice* renderdev, int x, int y, const std::string& text, std::vector<std::unique_ptr<GameTexture>>& font, GameTextAlignment alignment, int spacing, int red, int green, int blue)
{
	if (alignment == GameTextAlignment::Left)
	{
		DrawTextImpl(renderdev, x, y, std::string_view(text), font, alignment, spacing, red, green, blue);
	}
	else
	{
		auto linestart = text.begin();
		auto lineend = linestart;
		while (lineend != text.end())
		{
			uint8_t i = *lineend;
			if (i == '\n')
			{
				DrawTextImpl(renderdev, x, y, std::string_view(linestart, lineend), font, alignment, spacing, red, green, blue);
				y += font['X']->height + spacing;
				linestart = ++lineend;
			}
			else
			{
				++lineend;
			}
		}
		if (linestart != lineend)
		{
			DrawTextImpl(renderdev, x, y, std::string_view(linestart, lineend), font, alignment, spacing, red, green, blue);
		}
	}
}

void GameScreen::DrawTextImpl(RenderDevice* renderdev, int x, int y, const std::string_view& text, std::vector<std::unique_ptr<GameTexture>>& font, GameTextAlignment alignment, int spacing, int red, int green, int blue)
{
	if (alignment == GameTextAlignment::Center)
	{
		int textwidth = GetTextWidth(text, font, spacing);
		x -= textwidth / 2;
	}
	else if (alignment == GameTextAlignment::Right)
	{
		int textwidth = GetTextWidth(text, font, spacing);
		x -= textwidth;
	}

	int startx = x;
	for (char c : text)
	{
		uint8_t i = c;
		if (i == '\n')
		{
			x = startx;
			y += font['x']->height + spacing;
		}
		else if (i == 32 && font.size() > 'x')
		{
			x += font['x']->width + spacing;
		}
		else if (i < font.size() && !font[i]->pixels.empty())
		{
			renderdev->DrawImage(x, y, font[i].get(), red * (1.0f / 255.0f), green * (1.0f / 255.0f), blue * (1.0f / 255.0f));
			x += font[i]->width - font[i]->x + spacing;
		}
	}
}
