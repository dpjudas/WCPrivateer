#pragma once

#include "GameScreen.h"

class BaseScreen : public GameScreen
{
public:
	BaseScreen(GameApp* app);

	void Render(RenderDevice* renderdev) override;

	std::unique_ptr<GameTexture> background;
};
