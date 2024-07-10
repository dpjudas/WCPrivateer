#pragma once

#include "SceneScreen.h"

class MissionComputerScreen : public SceneScreen
{
public:
	MissionComputerScreen(GameApp* app);
	void Render(RenderDevice* renderdev) override;
	void OnClickTarget(WCTarget target) override;

	std::unique_ptr<GameTexture> computerscreen;
	std::vector<std::unique_ptr<GameTexture>> smallFont;
};
