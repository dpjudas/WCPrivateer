#pragma once

#include "GameScreen.h"

class OptionsScreen : public GameScreen
{
public:
	OptionsScreen(GameApp* app);
	~OptionsScreen();

	void OnKeyDown(InputKey key) override;
	void OnKeyUp(InputKey key) override;
	void Render(RenderDevice* renderdev) override;

	int GetHotButton();

	std::vector<std::unique_ptr<GameTexture>> background;
	std::vector<std::unique_ptr<GameTexture>> buttons;
	std::vector<std::unique_ptr<GameTexture>> font;

	int buttonAtMouseDown = -1;

	int buttonRects[10 * 4] =
	{
		15, 21, 99, 63,     // unlimitted ammo
		111,33, 210, 52,    // joystick calibration
		226, 21, 306, 63,   // invulernability
		15, 70, 96, 96,     // music
		228, 70, 306, 96,   // sound fx
		15, 124, 103, 169,  // mouse
		119, 148, 205, 170, // keyboard
		219, 124, 304, 170, // joystick
		11, 182, 159, 194,  // resume
		162, 182, 310, 194  // cancel
	};

	bool buttonState[10] =
	{
		false,
		false,
		false,
		true,
		true,
		false,
		true,
		false,
		false,
		false
	};
};
