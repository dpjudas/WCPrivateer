
#include "OptionsScreen.h"
#include "JoystickScreen.h"
#include "FileFormat/WCScene.h"
#include "Game/GameApp.h"

OptionsScreen::OptionsScreen(GameApp* app) : GameScreen(app)
{
}

OptionsScreen::~OptionsScreen()
{
}

void OptionsScreen::Render(RenderDevice* renderdev)
{
	if (background.empty())
	{
		auto palette = LoadPrefMainPalette();
		background = LoadShpImage("DATA\\OPTIONS\\PREFMAIN.SHP", palette.get());
		buttons = LoadShpImage("DATA\\OPTIONS\\PREFBUTT.SHP", palette.get());
	}

	renderdev->DrawImage(0, 0, background[0].get());

	for (int i = 0; i < 10; i++)
	{
		bool state = buttonState[i];
		if (i == buttonAtMouseDown && buttonAtMouseDown == GetHotButton())
		{
			state = !state;
		}

		int index = i * 2 + (state ? 1 : 0);
		renderdev->DrawImage(0, 0, buttons[index].get());
	}
}

void OptionsScreen::OnKeyDown(InputKey key)
{
	if (key == InputKey::LeftMouse)
	{
		buttonAtMouseDown = GetHotButton();
	}
}

void OptionsScreen::OnKeyUp(InputKey key)
{
	if (key == InputKey::LeftMouse)
	{
		if (buttonAtMouseDown == GetHotButton())
		{
			if (buttonAtMouseDown == 1) // joystick calibration
			{
				PushScreen(std::make_unique<JoystickScreen>(app));
			}
			else if (buttonAtMouseDown == 8) // resume
			{
				// To do: save toggle buttons state
				PopScreen();
			}
			else if (buttonAtMouseDown == 9) // cancel
			{
				PopScreen();
			}
			else // toggle buttons
			{
				buttonState[buttonAtMouseDown] = !buttonState[buttonAtMouseDown];
			}
		}
		buttonAtMouseDown = -1;
	}
}

int OptionsScreen::GetHotButton()
{
	for (int i = 0; i < 10; i++)
	{
		if (mouseX >= buttonRects[i * 4 + 0] && mouseY >= buttonRects[i * 4 + 1] && mouseX <= buttonRects[i * 4 + 2] && mouseY <= buttonRects[i * 4 + 3])
		{
			return i;
		}
	}
	return -1;
}
