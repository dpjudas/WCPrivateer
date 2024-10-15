
#include "TradeScreen.h"
#include "BaseScreen.h"
#include "Game/GameApp.h"

TradeScreen::TradeScreen(GameApp* app, const WCGameFlowTarget* target) : SceneScreen(app)
{
	nextScene = 63;
}

void TradeScreen::Render(RenderDevice* renderdev)
{
	SceneScreen::Render(renderdev);

	if (items.empty())
	{
		for (const auto& item : app->gamedata->commStuff)
		{
			items.push_back(LoadWCImage(*item, palette.get()));
		}

		// Where does those indexes come from?
		panel = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 204, palette.get());
		buttons = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 205, palette.get());

		smallFont = LoadMssgFont(palette.get(), 15); // The color of this font doesn't match the game. Is it actually the wrong font?
	}

	const WCCommodity& commodity = app->gamedata->commodityList->commodities[selectedItem];
	const std::vector<std::string>& commText = app->gamedata->commText;

	// Panel
	renderdev->DrawImage(0, 0, panel[0].get());
	renderdev->DrawImage(0, 0, buttons[mode].get());

	// Draw the current item
	if (commodity.info0 < items.size())
		renderdev->DrawImage(62, 40, items[commodity.info0][0].get());

	DrawText(renderdev, 65, 14, commodity.label, smallFont, GameTextAlignment::Center);
	if (!commodity.availability.empty())
		DrawText(renderdev, 63, 20, commText[(int)WCCommodityText::Quantity] + ": " + std::to_string(commodity.baseAvailability), smallFont, GameTextAlignment::Center);
	if (!commodity.cost.empty())
		DrawText(renderdev, 62, 66, commText[(int)WCCommodityText::Cost] + ": " + std::to_string(commodity.baseCost), smallFont, GameTextAlignment::Center);
	DrawText(renderdev, 62, 72, commText[(int)WCCommodityText::Credits] + ": 8847", smallFont, GameTextAlignment::Center);

	// DrawText(renderdev, 62, 42, commText[(int)WCCommodityText::Thanks], smallFont, GameTextAlignment::Center);
	// DrawText(renderdev, 62, 42, commText[(int)WCCommodityText::NoSell], smallFont, GameTextAlignment::Center);
	// DrawText(renderdev, 62, 42, commText[(int)WCCommodityText::NoBuy], smallFont, GameTextAlignment::Center);
	// DrawText(renderdev, 62, 42, commText[(int)WCCommodityText::NoCredit], smallFont, GameTextAlignment::Center);
	// DrawText(renderdev, 62, 42, commText[(int)WCCommodityText::NoRoom], smallFont, GameTextAlignment::Center);
}

void TradeScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<BaseScreen>(app, GetFlowTarget(target)));
	}
	else if (target == WCTarget::TradeNextItem)
	{
		selectedItem++;
		if (selectedItem == (int)app->gamedata->commodityList->commodities.size())
			selectedItem = 0;
	}
	else if (target == WCTarget::TradePrevItem)
	{
		selectedItem--;
		if (selectedItem == -1)
			selectedItem = (int)app->gamedata->commodityList->commodities.size() - 1;
	}
	else if (target == WCTarget::TradeSell)
	{
		mode = 0;
	}
	else if (target == WCTarget::TradeBuy)
	{
		mode = 1;
	}
	else if (target == WCTarget::TradeActivateItem)
	{
		if (mode == 1) // Buy item
		{
		}
		else if (mode == 0) // Sell item
		{
		}
	}
}
