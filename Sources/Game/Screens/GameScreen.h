#pragma once

#include "Game/RenderDevice.h"
#include "FileFormat/WCPalette.h"
#include <zwidget/core/widget.h>

class GameApp;
class GameTexture;
class WCImage;
class WCImageFrame;
class WCVOCSound;

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

	void PlayMusic(std::string filename, int song);
	void PlaySound(WCVOCSound* sound);

	int GetTextWidth(const std::string_view& text, std::vector<std::unique_ptr<GameTexture>>& font, int spacing = 2);
	std::string WordWrap(std::string text, int width, std::vector<std::unique_ptr<GameTexture>>& font, int spacing = 2);
	void DrawText(RenderDevice* renderdev, int x, int y, const std::string& text, std::vector<std::unique_ptr<GameTexture>>& font, GameTextAlignment alignment = GameTextAlignment::Left, int spacing = 2, int red = 255, int green = 255, int blue = 255);

	std::unique_ptr<WCPalette> LoadSpacePalette();
	std::unique_ptr<WCPalette> LoadPCMainPalette();
	std::unique_ptr<WCPalette> LoadPrefMainPalette();
	std::unique_ptr<WCPalette> LoadJoyCalibPalette();
	std::unique_ptr<WCPalette> LoadPalette(const std::string& filename);
	std::unique_ptr<WCPalette> LoadPakPalette(const std::string& pakFilename, int pakindex);

	std::vector<std::unique_ptr<GameTexture>> LoadFontsFont(WCPalette* palette, bool forceWhite = false);
	std::vector<std::unique_ptr<GameTexture>> LoadConvFont(WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadDemoFont(WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadMssgFont(WCPalette* palette, int color = -1);
	std::vector<std::unique_ptr<GameTexture>> LoadPCFont(WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadPrivFont(WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadOptFont(WCPalette* palette);

	std::vector<std::unique_ptr<GameTexture>> LoadPakImage(const std::string& pakFilename, int pakindex, const WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadShpImage(const std::string& filename, const WCPalette* palette, bool forceWhite = false);
	std::unique_ptr<GameTexture> LoadIffImage(const std::string& filename, int index, const WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadWCImage(const WCImage& image, const WCPalette* palette, bool forceWhite = false);

	std::vector<std::unique_ptr<GameTexture>> LoadDotImage(int red, int green, int blue, int alpha = 255);
	std::vector<std::unique_ptr<GameTexture>> LoadNavCircleImage(int red, int green, int blue, int alpha = 255);
	std::vector<std::unique_ptr<GameTexture>> LoadNavSquareImage(int red, int green, int blue, int alpha = 255);

	GameApp* app = nullptr;
	int mouseX = 0;
	int mouseY = 0;

private:
	void DrawTextImpl(RenderDevice* renderdev, int x, int y, const std::string_view& text, std::vector<std::unique_ptr<GameTexture>>& font, GameTextAlignment alignment = GameTextAlignment::Left, int spacing = 2, int red = 255, int green = 255, int blue = 255);
};
