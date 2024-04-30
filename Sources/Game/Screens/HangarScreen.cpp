
#include "HangarScreen.h"

HangarScreen::HangarScreen(GameApp* app) : GameScreen(app)
{
}

void HangarScreen::Render(RenderDevice* renderdev)
{
	if (!background)
	{
		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", 25);
		background = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 40, palette.get());
	}

	// Cars and people in background are missing

	renderdev->DrawImage(0, 0, 320, 200, background.get());
}
