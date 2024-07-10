#pragma once

#include "SceneScreen.h"

class ShipGarageScreen : public SceneScreen
{
public:
	ShipGarageScreen(GameApp* app);

	void OnClickTarget(WCTarget target) override;
	void Render(RenderDevice* renderdev) override;

	std::vector<std::unique_ptr<GameTexture>> shipShape;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> guns;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> misc;

	std::vector<std::unique_ptr<GameTexture>> panel;
	std::vector<std::unique_ptr<GameTexture>> buttons;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> shipStuff;
};
