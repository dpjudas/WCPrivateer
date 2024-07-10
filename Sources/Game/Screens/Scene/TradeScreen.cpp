
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
		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", app->gamedata->sceneList->scenes[scene].background.palette);
		for (const auto& item : app->gamedata->commStuff)
		{
			items.push_back(LoadWCImage(*item, palette.get()));
		}

		// Where does those indexes come from?
		panel = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 204, palette.get());
		buttons = LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", 205, palette.get());
	}

	// Panel
	renderdev->DrawImage(panel[0]->x, panel[0]->y, panel[0]->width, panel[0]->height, panel[0].get());
	renderdev->DrawImage(buttons[0]->x, buttons[0]->y, buttons[0]->width, buttons[0]->height, buttons[0].get());

	// Draw the current item
	{
		int item = 12;
		renderdev->DrawImage(62 + items[item][0]->x, 40 + items[item][0]->y, items[item][0]->width, items[item][0]->height, items[item][0].get());
	}
}

void TradeScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<BaseScreen>(app));
	}
}
