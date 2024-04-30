
#include "ShipDealerScreen.h"

ShipDealerScreen::ShipDealerScreen(GameApp* app) : GameScreen(app)
{
}

void ShipDealerScreen::Render(RenderDevice* renderdev)
{
	if (!background)
	{
		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", 3);
		background = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 3, palette.get()); // index 19 seems to be same picture but different palette?
	}

	renderdev->DrawImage(0, 0, 320, 200, background.get());
}
