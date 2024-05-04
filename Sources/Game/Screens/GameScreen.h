#pragma once

#include "Game/RenderDevice.h"
#include "FileFormat/WCPalette.h"
#include <zwidget/core/widget.h>

class GameApp;
class GameTexture;
class WCImage;

class GameScreen
{
public:
	GameScreen(GameApp* app);

	virtual void SetMousePos(int x, int y) { }
	virtual void OnKeyDown(InputKey key) { }
	virtual void Render(RenderDevice* device) { }

	std::unique_ptr<WCPalette> LoadSpacePalette();
	std::unique_ptr<WCPalette> LoadPCMainPalette();
	std::unique_ptr<WCPalette> LoadPrefMainPalette();
	std::unique_ptr<WCPalette> LoadJoyCalibPalette();
	std::unique_ptr<WCPalette> LoadPalette(const std::string& filename);
	std::unique_ptr<WCPalette> LoadPakPalette(const std::string& pakFilename, int pakindex);

	std::vector<std::unique_ptr<GameTexture>> LoadPakImage(const std::string& pakFilename, int pakindex, WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadShpImage(const std::string& filename, WCPalette* palette);
	std::unique_ptr<GameTexture> LoadIffImage(const std::string& filename, int index, WCPalette* palette);
	std::vector<std::unique_ptr<GameTexture>> LoadWCImage(const WCImage& image);

	GameApp* app = nullptr;
};
