#pragma once

#include "SceneScreen.h"

class BaseScreen : public SceneScreen
{
public:
	BaseScreen(GameApp* app, const WCGameFlowTarget* target);
	void OnClickTarget(WCTarget target) override;
};
