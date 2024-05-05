
#include "OptionsScreen.h"
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
		font = LoadShpImage("DATA\\FONTS\\OPTFONT.SHP", palette.get());
	}

	renderdev->DrawImage(background[0]->x, background[0]->y, background[0]->width, background[0]->height, background[0].get());

	for (int i = 0; i < 10; i++)
	{
		bool state = buttonState[i];
		if (i == buttonAtMouseDown && buttonAtMouseDown == GetHotButton())
		{
			state = !state;
		}

		int index = i * 2 + (state ? 1 : 0);
		renderdev->DrawImage(buttons[index]->x, buttons[index]->y, buttons[index]->width, buttons[index]->height, buttons[index].get());
	}

	std::string text = std::to_string(mouseX) + "," + std::to_string(mouseY);
	int textwidth = GetTextWidth(text, font);
	int x = (320 - textwidth) / 2;
	int y = 190;
	DrawText(renderdev, x, y, text, font);
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
				// To do: go to joystick calibration
			}
			else if (buttonAtMouseDown == 8) // resume
			{
				// To do: save toggle buttons state and go back
			}
			else if (buttonAtMouseDown == 9) // cancel
			{
				// To do: go back without saving
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
