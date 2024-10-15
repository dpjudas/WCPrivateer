#pragma once

#include "SceneScreen.h"

class MercenaryScreen : public SceneScreen
{
public:
	MercenaryScreen(GameApp* app, const WCGameFlowTarget* target);
	void Render(RenderDevice* renderdev) override;
	void OnClickTarget(WCTarget target) override;
};
