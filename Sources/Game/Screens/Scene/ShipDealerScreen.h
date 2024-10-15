#pragma once

#include "SceneScreen.h"

class ShipDealerScreen : public SceneScreen
{
public:
	ShipDealerScreen(GameApp* app, const WCGameFlowTarget* target);
	void OnClickTarget(WCTarget target) override;
};
