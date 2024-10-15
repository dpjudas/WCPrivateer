
#include "BaseScreen.h"
#include "HangarScreen.h"
#include "BarScreen.h"
#include "MercenaryScreen.h"
#include "MerchantScreen.h"
#include "MissionComputerScreen.h"
#include "ShipDealerScreen.h"
#include "TradeScreen.h"

BaseScreen::BaseScreen(GameApp* app, const WCGameFlowTarget* target) : SceneScreen(app)
{
	SetScene(target);
}

void BaseScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::LandingPad)
	{
		ShowScreen(std::make_unique<HangarScreen>(app));
	}
	else if (target == WCTarget::Bar)
	{
		ShowScreen(std::make_unique<BarScreen>(app, GetFlowTarget(target)));
	}
	else if (target == WCTarget::Library)
	{
	}
	else if (target == WCTarget::MercenariesGuild)
	{
		ShowScreen(std::make_unique<MercenaryScreen>(app, GetFlowTarget(target)));
	}
	else if (target == WCTarget::MerchantsGuild)
	{
		ShowScreen(std::make_unique<MerchantScreen>(app, GetFlowTarget(target)));
	}
	else if (target == WCTarget::MissionComputer)
	{
		ShowScreen(std::make_unique<MissionComputerScreen>(app, GetFlowTarget(target)));
	}
	else if (target == WCTarget::ShipDealer)
	{
		ShowScreen(std::make_unique<ShipDealerScreen>(app, GetFlowTarget(target)));
	}
	else if (target == WCTarget::CommodityExchange)
	{
		ShowScreen(std::make_unique<TradeScreen>(app, GetFlowTarget(target)));
	}
}
