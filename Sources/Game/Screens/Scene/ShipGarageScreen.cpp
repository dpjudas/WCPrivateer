
#include "ShipGarageScreen.h"
#include "ShipDealerScreen.h"
#include "ShipSoftwareScreen.h"
#include "Game/GameApp.h"

ShipGarageScreen::ShipGarageScreen(GameApp* app) : SceneScreen(app)
{
	nextScene = 69;
	regionTextX = 240;
}

void ShipGarageScreen::Render(RenderDevice* renderdev)
{
	SceneScreen::Render(renderdev);

	if (shipStuff.empty())
	{
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

		smallFont = LoadMssgFont(palette.get(), 15); // The color of this font doesn't match the game. Is it actually the wrong font?
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
	renderdev->DrawImage(panel[0]->x, 199 - panel[0]->y - panel[0]->height, panel[0]->width, panel[0]->height, panel[0].get());

	// Active mode
	renderdev->DrawImage(buttons[mode]->x, 199 - buttons[mode]->y - buttons[mode]->height, buttons[mode]->width, buttons[mode]->height, buttons[mode].get());

	// Draw the current item
	renderdev->DrawImage(75 + shipStuff[selectedItem][0]->x, 150 + shipStuff[selectedItem][0]->y, shipStuff[selectedItem][0]->width, shipStuff[selectedItem][0]->height, shipStuff[selectedItem][0].get());

	const std::vector<std::string>& mtext = app->gamedata->shipMText;
	const WCShipStuffItem& itemInfo = app->gamedata->shipStuff[selectedItem];
	int damage = 50;

	DrawText(renderdev, 65, 121, itemInfo.label, smallFont, GameTextAlignment::Center);
	DrawText(renderdev, 65, 128, mtext[(int)WCShipMText::Quantity] + std::to_string(2), smallFont, GameTextAlignment::Center);
	if (mode == 0)
		DrawText(renderdev, 65, 175, mtext[(int)WCShipMText::Cost] + std::to_string(itemInfo.buyprice), smallFont, GameTextAlignment::Center);
	else if (mode == 1)
		DrawText(renderdev, 65, 175, mtext[(int)WCShipMText::Cost] + std::to_string(itemInfo.sellprice), smallFont, GameTextAlignment::Center);
	else if (mode == 2)
		DrawText(renderdev, 65, 175, mtext[(int)WCShipMText::Cost] + std::to_string(itemInfo.sellprice * 100 / damage), smallFont, GameTextAlignment::Center);
	DrawText(renderdev, 65, 181, mtext[(int)WCShipMText::Credits] + std::to_string(8847), smallFont, GameTextAlignment::Center);

	if (mode == 2)
		DrawText(renderdev, 63, 150, mtext[(int)WCShipMText::NoRepair], smallFont, GameTextAlignment::Center);
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
	else if (target == WCTarget::GarageSell)
	{
		mode = 1;
	}
	else if (target == WCTarget::GarageBuy)
	{
		mode = 0;
	}
	else if (target == WCTarget::GarageRepair)
	{
		mode = 2;
	}
	else if (target == WCTarget::GarageNextItem)
	{
		selectedItem++;
		if (selectedItem == (int)shipStuff.size())
			selectedItem = 0;
	}
	else if (target == WCTarget::GaragePrevItem)
	{
		selectedItem--;
		if (selectedItem == -1)
			selectedItem = (int)shipStuff.size() - 1;
	}
	else if (target == WCTarget::GarageActivateItem)
	{
		if (mode == 0) // Buy item
		{
		}
		else if (mode == 1) // Sell item
		{
		}
		else if (mode == 2) // Repair item
		{
		}
	}
}
