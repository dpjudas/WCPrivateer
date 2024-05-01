
#include "MainMenuScreen.h"
#include "FileFormat/WCScene.h"
#include "Game/GameApp.h"

MainMenuScreen::MainMenuScreen(GameApp* app) : GameScreen(app)
{
}

MainMenuScreen::~MainMenuScreen()
{
}

void MainMenuScreen::Render(RenderDevice* renderdev)
{
	if (!background)
	{
		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", 39);
		background = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 181, palette.get());
	}

	renderdev->DrawImage(background->x, background->y, background->width, background->height, background.get());
}
