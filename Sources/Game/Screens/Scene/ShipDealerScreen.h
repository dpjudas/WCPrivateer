#pragma once

#include "SceneScreen.h"

class ShipDealerScreen : public SceneScreen
{
public:
	ShipDealerScreen(GameApp* app);
	void OnClickTarget(WCTarget target) override;
};
