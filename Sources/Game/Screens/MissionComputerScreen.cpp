
#include "MissionComputerScreen.h"

MissionComputerScreen::MissionComputerScreen(GameApp* app) : GameScreen(app)
{
}

void MissionComputerScreen::Render(RenderDevice* renderdev)
{
	if (!background)
	{
		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", 2);
		background = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 2, palette.get());
	}

	renderdev->DrawImage(0, 0, 320, 200, background.get());
}
