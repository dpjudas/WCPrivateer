#pragma once

#include "SceneScreen.h"

class BarScreen : public SceneScreen
{
public:
	BarScreen(GameApp* app, const WCGameFlowTarget* target);
	void Render(RenderDevice* renderdev) override;
	void OnClickTarget(WCTarget target) override;
};
