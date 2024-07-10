
#include "ShipDealerScreen.h"
#include "BaseScreen.h"
#include "ShipGarageScreen.h"

ShipDealerScreen::ShipDealerScreen(GameApp* app) : SceneScreen(app)
{
	nextScene = 45;
}

void ShipDealerScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<BaseScreen>(app));
	}
	else if (target == WCTarget::RepairAndUpgrade)
	{
		ShowScreen(std::make_unique<ShipGarageScreen>(app));
	}
}
