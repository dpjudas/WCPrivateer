#pragma once

#include "GameScreen.h"

class ShipDealerScreen : public GameScreen
{
public:
	ShipDealerScreen(GameApp* app);

	void Render(RenderDevice* renderdev) override;

	std::unique_ptr<GameTexture> background;
};
