
#include "LoadSaveScreen.h"

LoadSaveScreen::LoadSaveScreen(GameApp* app) : GameScreen(app)
{
}

void LoadSaveScreen::Render(RenderDevice* renderdev)
{
	if (!background)
	{
		auto palette = LoadPCMainPalette();
		background = LoadShpImage("DATA\\OPTIONS\\LOADSAVE.SHP", 0, palette.get()); // frames 1-11 are the various buttons in different states
	}

	renderdev->DrawImage(0, 0, 320, 200, background.get());
}
