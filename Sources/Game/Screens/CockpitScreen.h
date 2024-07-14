#pragma once

#include "GameScreen.h"

class WCCockpit;

class CockpitScreen : public GameScreen
{
public:
	CockpitScreen(GameApp* app);
	~CockpitScreen();

	void Render(RenderDevice* renderdev) override;

	std::unique_ptr<WCCockpit> cockpit;
	std::unique_ptr<WCPalette> palette;
	std::vector<std::unique_ptr<GameTexture>> front;
	int framecounter = 0;
};
