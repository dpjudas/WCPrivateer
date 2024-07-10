#pragma once

#include "SceneScreen.h"

class BaseScreen : public SceneScreen
{
public:
	BaseScreen(GameApp* app);
	void OnClickTarget(WCTarget target) override;
};
