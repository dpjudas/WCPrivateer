
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
		for (const auto& item : app->gamedata->softwareStuff)
		{
			softwareStuff.push_back(LoadWCImage(*item.shape, palette.get()));
		}

		smallFont = LoadMssgFont(palette.get(), 15); // The color of this font doesn't match the game. Is it actually the wrong font?
	}

	renderdev->DrawImage(65 + softwareStuff[selectedItem][0]->x, 120 + softwareStuff[selectedItem][0]->y, softwareStuff[selectedItem][0]->width, softwareStuff[selectedItem][0]->height, softwareStuff[selectedItem][0].get());

	const std::vector<std::string>& softText = app->gamedata->softText;
	const WCShipStuffItem& itemInfo = app->gamedata->softwareStuff[selectedItem];
	int damage = 50;

	DrawText(renderdev, 60, 89, itemInfo.label, smallFont, GameTextAlignment::Center);
	DrawText(renderdev, 60, 151, softText[(int)WCSoftwareText::Credits] + std::to_string(8847), smallFont, GameTextAlignment::Center);

	if (mode == 2)
		DrawText(renderdev, 58, 120, softText[(int)WCSoftwareText::NoRepair], smallFont, GameTextAlignment::Center);

	if (mode == 0)
		DrawText(renderdev, 23, 157, softText[(int)WCSoftwareText::Mode] + softText[(int)WCSoftwareText::Buy], smallFont, GameTextAlignment::Left);
	else if (mode == 1)
		DrawText(renderdev, 23, 157, softText[(int)WCSoftwareText::Mode] + softText[(int)WCSoftwareText::Sell], smallFont, GameTextAlignment::Left);
	else if (mode == 2)
		DrawText(renderdev, 23, 157, softText[(int)WCSoftwareText::Mode] + softText[(int)WCSoftwareText::Repair], smallFont, GameTextAlignment::Left);

	if (mode == 0)
		DrawText(renderdev, 105, 157, std::to_string(itemInfo.buyprice), smallFont, GameTextAlignment::Right);
	else if (mode == 1)
		DrawText(renderdev, 105, 157, std::to_string(itemInfo.sellprice), smallFont, GameTextAlignment::Right);
	else if (mode == 2)
		DrawText(renderdev, 105, 157, std::to_string(itemInfo.sellprice * 100 / damage), smallFont, GameTextAlignment::Right);
}

void ShipSoftwareScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<ShipGarageScreen>(app));
	}
	else if (target == WCTarget::SoftwareSell)
	{
		mode = 1;
	}
	else if (target == WCTarget::SoftwareBuy)
	{
		mode = 0;
	}
	else if (target == WCTarget::SoftwareRepair)
	{
		mode = 2;
	}
	else if (target == WCTarget::SoftwareNextItem)
	{
		selectedItem++;
		if (selectedItem == (int)softwareStuff.size())
			selectedItem = 0;
	}
	else if (target == WCTarget::SoftwarePrevItem)
	{
		selectedItem--;
		if (selectedItem == -1)
			selectedItem = (int)softwareStuff.size() - 1;
	}
	else if ((int)target >= (int)WCTarget::SoftwareFirstItem && (int)target <= (int)WCTarget::SoftwareLastItem)
	{
		selectedItem = (int)target - (int)WCTarget::SoftwareFirstItem - 5;
		if (selectedItem < 0)
			selectedItem += (int)softwareStuff.size();
	}
	else if (target == WCTarget::SoftwareActivateItem)
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
