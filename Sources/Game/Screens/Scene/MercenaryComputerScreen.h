#pragma once

#include "SceneScreen.h"

class MercenaryComputerScreen : public SceneScreen
{
public:
	MercenaryComputerScreen(GameApp* app, const WCGameFlowTarget* target);
	void Render(RenderDevice* renderdev) override;
	void OnClickTarget(WCTarget target) override;
};
