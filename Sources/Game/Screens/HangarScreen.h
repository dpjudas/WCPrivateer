#pragma once

#include "GameScreen.h"

class HangarScreen : public GameScreen
{
public:
	HangarScreen(GameApp* app);

	void Render(RenderDevice* renderdev) override;

	std::unique_ptr<GameTexture> background;
};
