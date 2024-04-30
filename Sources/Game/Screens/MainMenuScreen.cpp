
#include "MainMenuScreen.h"
#include "FileFormat/WCScene.h"
#include "Game/GameApp.h"

MainMenuScreen::MainMenuScreen(GameApp* app) : GameScreen(app)
{
}

void MainMenuScreen::Render(RenderDevice* renderdev)
{
	if (!background)
	{
		WCSceneList options(app->archive.get());

		//auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", 39);
		//background = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 181, palette.get());

		int scene = 0;
		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", options.scenes[scene].background.palette);
		background = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", options.scenes[scene].background.shapes[0].optpakIndex, palette.get());
	}

	renderdev->DrawImage(0, 0, 320, 200, background.get());
}
