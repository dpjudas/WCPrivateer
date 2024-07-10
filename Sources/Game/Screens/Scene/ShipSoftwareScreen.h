#pragma once

#include "SceneScreen.h"

class ShipSoftwareScreen : public SceneScreen
{
public:
	ShipSoftwareScreen(GameApp* app);

	void OnClickTarget(WCTarget target) override;
	void Render(RenderDevice* renderdev) override;

	std::vector<std::vector<std::unique_ptr<GameTexture>>> softwareStuff;
	std::vector<std::unique_ptr<GameTexture>> smallFont;

	int mode = 0;
	int selectedItem = 0;
};
