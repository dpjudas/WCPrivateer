#pragma once

#include "SceneScreen.h"

class HangarScreen : public SceneScreen
{
public:
	HangarScreen(GameApp* app);
	void OnClickTarget(WCTarget target) override;
};
