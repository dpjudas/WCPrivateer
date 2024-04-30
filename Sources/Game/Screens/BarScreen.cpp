
#include "BarScreen.h"

BarScreen::BarScreen(GameApp* app) : GameScreen(app)
{
}

void BarScreen::Render(RenderDevice* renderdev)
{
	if (!background)
	{
		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", 4);
		background = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 4, palette.get());

		//auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", 19);
		//background = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 25, palette.get());
	}

	// Table is missing
	// Bartender is missing
	// Guests in the back are missing

	renderdev->DrawImage(0, 0, 320, 200, background.get());
}
