#pragma once

#include "Game/RenderDevice.h"
#include "FileFormat/WCPalette.h"
#include <zwidget/core/widget.h>

class GameApp;
class GameTexture;
class WCImage;
class WCImageFrame;

enum class GameTextAlignment
{
	Left,
	Center,
	Right
};

class GameScreen
{
public:
	GameScreen(GameApp* app);

	void SetMousePos(int x, int y)
	{
		mouseX = x;
		mouseY = y;
	}

	virtual void OnKeyDown(InputKey key) { }
	virtual void OnKeyUp(InputKey key) { }
	virtual void Render(RenderDevice* device) { }

	void ShowScreen(std::unique_ptr<GameScreen> screen);
	void PushScreen(std::unique_ptr<GameScreen> screen);
	void PopScreen();

	int GetTextWidth(const std::string_view& text, std::vector<std::unique_ptr<GameTexture>>& font);
	std::string WordWrap(std::string text, int width, std::vector<std::unique_ptr<GameTexture>>& font);
	void DrawText(RenderDevice* renderdev, int x, int y, const std::string& text, std::vector<std::unique_ptr<GameTexture>>& font, GameTextAlignment alignment = GameTextAlignment::Left);

	std::unique_ptr<WCPalette> LoadSpacePalette();
	std::unique_ptr<WCPalette> LoadPCMainPalette();
	std::unique_ptr<WCPalette> LoadPrefMainPalette();
	std::unique_ptr<WCPalette> LoadJoyCalibPalette();
	std::unique_ptr<WCPalette> LoadPalette(const std::string& filename);
	std::unique_ptr<WCPalette> LoadPakPalette(const std::string& pakFilename, int pakindex);

	std::vector<std::unique_ptr<GameTexture>> LoadFontsFont(WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadConvFont(WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadDemoFont(WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadMssgFont(WCPalette* palette, int color = -1);
	std::vector<std::unique_ptr<GameTexture>> LoadPCFont(WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadPrivFont(WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadOptFont(WCPalette* palette);

	std::vector<std::unique_ptr<GameTexture>> LoadPakImage(const std::string& pakFilename, int pakindex, const WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadShpImage(const std::string& filename, const WCPalette* palette);
	std::unique_ptr<GameTexture> LoadIffImage(const std::string& filename, int index, const WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadWCImage(const WCImage& image, const WCPalette* palette);

	GameApp* app = nullptr;
	int mouseX = 0;
	int mouseY = 0;

private:
	void DrawTextImpl(RenderDevice* renderdev, int x, int y, const std::string_view& text, std::vector<std::unique_ptr<GameTexture>>& font, GameTextAlignment alignment = GameTextAlignment::Left);
};
