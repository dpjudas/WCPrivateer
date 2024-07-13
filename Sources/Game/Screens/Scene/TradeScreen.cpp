
#include "TradeScreen.h"
#include "BaseScreen.h"
#include "Game/GameApp.h"

TradeScreen::TradeScreen(GameApp* app) : SceneScreen(app)
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
	renderdev->DrawImage(panel[0]->x, panel[0]->y, panel[0]->width, panel[0]->height, panel[0].get());
	renderdev->DrawImage(buttons[mode]->x, buttons[mode]->y, buttons[mode]->width, buttons[mode]->height, buttons[mode].get());

	// Draw the current item
	if (commodity.info0 < items.size())
		renderdev->DrawImage(62 + items[commodity.info0][0]->x, 40 + items[commodity.info0][0]->y, items[commodity.info0][0]->width, items[commodity.info0][0]->height, items[commodity.info0][0].get());

	DrawText(renderdev, 65, 14, commodity.label, smallFont, GameTextAlignment::Center);
	if (!commodity.availability.empty())
		DrawText(renderdev, 63, 20, commText[(int)WCCommodityText::Quantity] + ": " + std::to_string(commodity.availability.front()), smallFont, GameTextAlignment::Center);
	if (!commodity.cost.empty())
		DrawText(renderdev, 62, 66, commText[(int)WCCommodityText::Cost] + ": " + std::to_string(commodity.cost.front()), smallFont, GameTextAlignment::Center);
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
		ShowScreen(std::make_unique<BaseScreen>(app));
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
