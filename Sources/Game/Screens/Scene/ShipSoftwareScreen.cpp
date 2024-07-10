
#include "ShipSoftwareScreen.h"
#include "ShipGarageScreen.h"
#include "Game/GameApp.h"

ShipSoftwareScreen::ShipSoftwareScreen(GameApp* app) : SceneScreen(app)
{
	nextScene = 64;
}

void ShipSoftwareScreen::Render(RenderDevice* renderdev)
{
	SceneScreen::Render(renderdev);
	if (softwareStuff.empty())
	{
		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", app->gamedata->sceneList->scenes[scene].background.palette);
		for (const auto& item : app->gamedata->softwareStuff)
		{
			softwareStuff.push_back(LoadWCImage(*item.shape, palette.get()));
		}
	}

	int item = 4;
	renderdev->DrawImage(65 + softwareStuff[item][0]->x, 120 + softwareStuff[item][0]->y, softwareStuff[item][0]->width, softwareStuff[item][0]->height, softwareStuff[item][0].get());
}

void ShipSoftwareScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<ShipGarageScreen>(app));
	}
}
