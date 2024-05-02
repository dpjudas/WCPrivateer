
#include "LoadSaveScreen.h"

LoadSaveScreen::LoadSaveScreen(GameApp* app) : GameScreen(app)
{
}

void LoadSaveScreen::Render(RenderDevice* renderdev)
{
	if (background.empty())
	{
		auto palette = LoadPCMainPalette();
		background = LoadShpImage("DATA\\OPTIONS\\LOADSAVE.SHP", palette.get()); // frames 1-11 are the various buttons in different states
	}

	renderdev->DrawImage(0, 0, 320, 200, background[0].get());
}
