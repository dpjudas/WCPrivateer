
#include "ShipDealerScreen.h"
#include "BaseScreen.h"
#include "ShipGarageScreen.h"
#include "Game/Screens/ConversationScreen.h"

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
	else if (target == WCTarget::PurchaseCenturion)
	{
		ShowScreen(std::make_unique<ConversationScreen>(app, "TOOPOOR"));
	}
	else if (target == WCTarget::PurchaseOrion)
	{
		ShowScreen(std::make_unique<ConversationScreen>(app, "TOOPOOR"));
	}
	else if (target == WCTarget::PurchaseGalaxy)
	{
		ShowScreen(std::make_unique<ConversationScreen>(app, "TOOPOOR"));
	}
}
