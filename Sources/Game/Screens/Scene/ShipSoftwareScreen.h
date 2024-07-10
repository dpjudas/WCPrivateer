#pragma once

#include "SceneScreen.h"

class ShipSoftwareScreen : public SceneScreen
{
public:
	ShipSoftwareScreen(GameApp* app);

	void OnClickTarget(WCTarget target) override;
	void Render(RenderDevice* renderdev) override;

	std::vector<std::vector<std::unique_ptr<GameTexture>>> softwareStuff;
};
