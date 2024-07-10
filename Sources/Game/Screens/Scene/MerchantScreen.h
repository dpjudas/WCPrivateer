#pragma once

#include "SceneScreen.h"

class MerchantScreen : public SceneScreen
{
public:
	MerchantScreen(GameApp* app);
	void Render(RenderDevice* renderdev) override;
	void OnClickTarget(WCTarget target) override;
};
