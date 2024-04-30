
#include "BaseScreen.h"

BaseScreen::BaseScreen(GameApp* app) : GameScreen(app)
{
}

void BaseScreen::Render(RenderDevice* renderdev)
{
	if (!background)
	{
		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", 34);
		background = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 78, palette.get());
	}

	// Cars and people in background are missing

	renderdev->DrawImage(0, 0, 320, 200, background.get());
}
