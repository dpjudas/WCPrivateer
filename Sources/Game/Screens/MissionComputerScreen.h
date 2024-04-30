#pragma once

#include "GameScreen.h"

class MissionComputerScreen : public GameScreen
{
public:
	MissionComputerScreen(GameApp* app);

	void Render(RenderDevice* renderdev) override;

	std::unique_ptr<GameTexture> background;
};
