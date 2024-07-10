
#include "ShipGarageScreen.h"
#include "ShipDealerScreen.h"
#include "ShipSoftwareScreen.h"
#include "Game/GameApp.h"

ShipGarageScreen::ShipGarageScreen(GameApp* app) : SceneScreen(app)
{
	nextScene = 69;
}

void ShipGarageScreen::Render(RenderDevice* renderdev)
{
	SceneScreen::Render(renderdev);

	if (shipStuff.empty())
	{
		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", app->gamedata->sceneList->scenes[scene].background.palette);
		for (const auto& item : app->gamedata->shipStuff)
		{
			shipStuff.push_back(LoadWCImage(*item.shape, palette.get()));
		}

		// Where does those indexes come from?
		panel = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 24, palette.get());
		buttons = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 203, palette.get());

		WCShipArt& shipArt = (++app->gamedata->shipArt.begin())->second;
		shipShape = LoadWCImage(*shipArt.shape, palette.get());
		for (const auto& item : shipArt.guns)
		{
			guns.push_back(LoadWCImage(*item.shape, palette.get()));
		}
		for (const auto& item : shipArt.misc)
		{
			misc.push_back(LoadWCImage(*item.shape, palette.get()));
		}
	}

	// Draw the ship
	renderdev->DrawImage(shipShape[0]->x, shipShape[0]->y, shipShape[0]->width, shipShape[0]->height, shipShape[0].get());

	// Draw guns
	{
		int item = 5;
		for (int slot = 0; slot < 4; slot++)
		{
			renderdev->DrawImage(guns[item][slot]->x, guns[item][slot]->y, guns[item][slot]->width, guns[item][slot]->height, guns[item][slot].get());
		}
	}

	// Software module
	{
		int item = 2;
		int subitem = 2;
		renderdev->DrawImage(misc[item][subitem]->x, misc[item][subitem]->y, misc[item][subitem]->width, misc[item][subitem]->height, misc[item][subitem].get());
	}

	// Panel
	renderdev->DrawImage(panel[0]->x, 200 - panel[0]->y - panel[0]->height, panel[0]->width, panel[0]->height, panel[0].get());
	renderdev->DrawImage(buttons[2]->x, 200 - buttons[2]->y - buttons[2]->height, buttons[2]->width, buttons[2]->height, buttons[2].get());

	// Draw the current item
	{
		int item = 10;
		renderdev->DrawImage(75 + shipStuff[item][0]->x, 150 + shipStuff[item][0]->y, shipStuff[item][0]->width, shipStuff[item][0]->height, shipStuff[item][0].get());
	}
}

void ShipGarageScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<ShipDealerScreen>(app));
	}
	else if (target == WCTarget::SoftwareBooth)
	{
		ShowScreen(std::make_unique<ShipSoftwareScreen>(app));
	}
}
