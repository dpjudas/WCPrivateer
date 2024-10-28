#pragma once

#include "Game/Screens/GameScreen.h"

class WCMovie;

class LandingScreen : public GameScreen
{
public:
	LandingScreen(GameApp* app);
	~LandingScreen();

	void Render(RenderDevice* renderdev) override;

	std::unique_ptr<WCMovie> movie;
	std::unique_ptr<WCPalette> palette;
	std::vector<std::unique_ptr<GameTexture>> background;
	std::vector<std::unique_ptr<GameTexture>> base;
	std::vector<std::unique_ptr<GameTexture>> ship;

	int64_t startTime = 0;
	int curTime = 0;
};
