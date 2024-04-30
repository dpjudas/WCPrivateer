#pragma once

#include "GameScreen.h"

class LoadSaveScreen : public GameScreen
{
public:
	LoadSaveScreen(GameApp* app);

	void Render(RenderDevice* renderdev) override;

	std::unique_ptr<GameTexture> background;
};
