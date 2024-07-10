
#include "LoadSaveScreen.h"
#include "Scene/HangarScreen.h"

LoadSaveScreen::LoadSaveScreen(GameApp* app, LoadSaveScreenState state) : GameScreen(app), state(state)
{
}

void LoadSaveScreen::Render(RenderDevice* renderdev)
{
	if (background.empty())
	{
		auto palette = LoadPCMainPalette();
		background = LoadShpImage("DATA\\OPTIONS\\LOADSAVE.SHP", palette.get());
		font = LoadPCFont(palette.get());
	}

	renderdev->DrawImage(background[0]->x, background[0]->y, background[0]->width, background[0]->height, background[0].get());

	ButtonName cursorButton = GetHotButton();
	if (buttonAtMouseDown == cursorButton && cursorButton != ButtonName::None)
	{
		int index = 2 + (int)cursorButton;
		renderdev->DrawImage(background[index]->x, background[index]->y, background[index]->width, background[index]->height, background[index].get());
	}

	/*
	std::string text = std::to_string(mouseX) + "," + std::to_string(mouseY);
	int textwidth = GetTextWidth(text, font);
	int x = (320 - textwidth) / 2;
	int y = 190;
	DrawText(renderdev, x, y, text, font);
	*/

	if (state == LoadSaveScreenState::New)
	{
		DrawText(renderdev, 30, 40, "Please register your", font);
		DrawText(renderdev, 30, 40 + 1 * 8, "   new Quine 4000", font);
		DrawText(renderdev, 30, 40 + 3 * 8, "Enter Name:", font);
		DrawText(renderdev, 30, 40 + 4 * 8 + 4, " _", font);
		DrawText(renderdev, 30, 40 + 6 * 8, "Enter Callsign:", font);
		// DrawText(renderdev, 30, 40 + 7 * 8 + 4, " _", font);
	}
	else if (state == LoadSaveScreenState::Load)
	{
		DrawText(renderdev, 30, 40, "Select game:", font);
	}
	else if (state == LoadSaveScreenState::Save)
	{
	}
}

void LoadSaveScreen::OnKeyDown(InputKey key)
{
	if (key == InputKey::LeftMouse)
	{
		buttonAtMouseDown = GetHotButton();
	}
}

void LoadSaveScreen::OnKeyUp(InputKey key)
{
	if (key == InputKey::LeftMouse)
	{
		buttonAtMouseDown = ButtonName::None;

		ButtonName action = GetHotButton();
		if (action == ButtonName::Save)
		{
		}
		else if (action == ButtonName::Load)
		{
			ShowScreen(std::make_unique<HangarScreen>(app));
		}
		else if (action == ButtonName::Missions)
		{
		}
		else if (action == ButtonName::Finances)
		{
		}
		else if (action == ButtonName::Manifest)
		{
		}
		else if (action == ButtonName::UpArrow)
		{
		}
		else if (action == ButtonName::DownArrow)
		{
		}
		else if (action == ButtonName::LeftArrow)
		{
		}
		else if (action == ButtonName::RightArrow)
		{
		}
		else if (action == ButtonName::Power)
		{
			PopScreen();
		}
	}
}

LoadSaveScreen::ButtonName LoadSaveScreen::GetHotButton()
{
	for (int i = 0; i < 10; i++)
	{
		if (mouseX >= buttonRects[i * 4 + 0] && mouseY >= buttonRects[i * 4 + 1] && mouseX <= buttonRects[i * 4 + 2] && mouseY <= buttonRects[i * 4 + 3])
		{
			return (ButtonName)i;
		}
	}
	return ButtonName::None;
}
