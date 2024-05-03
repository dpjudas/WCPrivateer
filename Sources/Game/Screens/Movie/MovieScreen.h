#pragma once

#include "Game/Screens/GameScreen.h"

class WCMovie;

class MovieScreen : public GameScreen
{
public:
	MovieScreen(GameApp* app);
	~MovieScreen();

	void OnKeyDown(InputKey key) override;
	void Render(RenderDevice* renderdev) override;

	std::unique_ptr<WCMovie> movie;
	std::vector<std::unique_ptr<GameTexture>> frames[3];
};
