
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
	if (background.empty())
	{
		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", 39);
		background = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 181, palette.get());
	}

	renderdev->DrawImage(background[0]->x, background[0]->y, background[0]->width, background[0]->height, background[0].get());
}
