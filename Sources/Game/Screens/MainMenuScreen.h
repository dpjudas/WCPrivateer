#pragma once

#include "GameScreen.h"

class MainMenuScreen : public GameScreen
{
public:
	MainMenuScreen(GameApp* app);
	~MainMenuScreen();

	void Render(RenderDevice* renderdev) override;

	std::unique_ptr<GameTexture> background;
};
