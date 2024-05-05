#pragma once

#include "GameScreen.h"

class JoystickScreen : public GameScreen
{
public:
	JoystickScreen(GameApp* app);

	void OnKeyUp(InputKey key) override;
	void Render(RenderDevice* renderdev) override;

	std::vector<std::unique_ptr<GameTexture>> background;
	int calibrationStep = 1;
};
