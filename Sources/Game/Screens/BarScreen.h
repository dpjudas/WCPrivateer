#pragma once

#include "GameScreen.h"

class BarScreen : public GameScreen
{
public:
	BarScreen(GameApp* app);

	void Render(RenderDevice* renderdev) override;

	std::unique_ptr<GameTexture> background;
};
