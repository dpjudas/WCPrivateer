#pragma once

#include "SceneScreen.h"

class MerchantComputerScreen : public SceneScreen
{
public:
	MerchantComputerScreen(GameApp* app, const WCGameFlowTarget* target);
	void Render(RenderDevice* renderdev) override;
	void OnClickTarget(WCTarget target) override;
};
