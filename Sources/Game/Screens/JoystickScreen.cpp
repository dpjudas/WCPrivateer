
#include "JoystickScreen.h"

JoystickScreen::JoystickScreen(GameApp* app) : GameScreen(app)
{
}

void JoystickScreen::OnKeyUp(InputKey key)
{
	if (key == InputKey::Joy1 || key == InputKey::LeftMouse)
	{
		if (calibrationStep < 7)
			calibrationStep++;
		else
			PopScreen();
	}
}

void JoystickScreen::Render(RenderDevice* renderdev)
{
	if (background.empty())
	{
		auto palette = LoadJoyCalibPalette();
		background = LoadShpImage("DATA\\OPTIONS\\JOYCALIB.SHP", palette.get());
	}

	renderdev->DrawImage(3, 195 - background[0]->height, background[0].get());
	renderdev->DrawImage((320 - background[calibrationStep]->width) / 2 - 2, 195 - 33, background[calibrationStep].get());
}
