
#include "ShipGarageScreen.h"
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
	renderdev->DrawImage(0, 0, shipShape[0].get());

	// Draw guns
	{
		int item = 5;
		for (int slot = 0; slot < 4; slot++)
		{
			renderdev->DrawImage(0, 0, guns[item][slot].get());
		}
	}

	// Software module
	{
		int item = 2;
		int subitem = 2;
		renderdev->DrawImage(0, 0, misc[item][subitem].get());
	}

	// Panel
	renderdev->DrawImage(0, 199 - panel[0]->height, panel[0].get());

	// Active mode
	renderdev->DrawImage(0, 199 - buttons[mode]->height, buttons[mode].get());

	// Draw the current item
	renderdev->DrawImage(75, 150, shipStuff[selectedItem][0].get());

	const std::vector<std::string>& mtext = app->gamedata->shipMText;
	const WCShipStuffItem& itemInfo = app->gamedata->shipStuff[selectedItem];
	int damage = 50;

	DrawText(renderdev, 72, 121, itemInfo.label, smallFont, GameTextAlignment::Center);
	DrawText(renderdev, 72, 128, mtext[(int)WCShipMText::Quantity] + std::to_string(2), smallFont, GameTextAlignment::Center);
	if (mode == 0)
		DrawText(renderdev, 72, 175, mtext[(int)WCShipMText::Cost] + std::to_string(itemInfo.buyprice), smallFont, GameTextAlignment::Center);
	else if (mode == 1)
		DrawText(renderdev, 72, 175, mtext[(int)WCShipMText::Cost] + std::to_string(itemInfo.sellprice), smallFont, GameTextAlignment::Center);
	else if (mode == 2)
		DrawText(renderdev, 72, 175, mtext[(int)WCShipMText::Cost] + std::to_string(itemInfo.sellprice * (100 - damage) / 200), smallFont, GameTextAlignment::Center);
	DrawText(renderdev, 72, 181, mtext[(int)WCShipMText::Credits] + std::to_string(8847), smallFont, GameTextAlignment::Center);

	if (mode == 2)
		DrawText(renderdev, 72, 150, mtext[(int)WCShipMText::NoRepair], smallFont, GameTextAlignment::Center);
}

void ShipGarageScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		PopScreen();
	}
	else if (target == WCTarget::SoftwareBooth)
	{
		PushScreen(std::make_unique<ShipSoftwareScreen>(app));
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
