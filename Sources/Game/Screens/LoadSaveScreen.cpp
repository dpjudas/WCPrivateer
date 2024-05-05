
#include "LoadSaveScreen.h"

LoadSaveScreen::LoadSaveScreen(GameApp* app) : GameScreen(app)
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

	if (buttonAtMouseDown != 0)
	{
		int index = GetHotButton();
		if (index == buttonAtMouseDown)
			renderdev->DrawImage(background[index]->x, background[index]->y, background[index]->width, background[index]->height, background[index].get());
	}

	/*
	std::string text = std::to_string(mouseX) + "," + std::to_string(mouseY);
	int textwidth = GetTextWidth(text, font);
	int x = (320 - textwidth) / 2;
	int y = 190;
	DrawText(renderdev, x, y, text, font);
	*/

	DrawText(renderdev, 30, 40, "Please register your", font);
	DrawText(renderdev, 30, 40 + 1 * 8, "   new Quine 4000", font);
	DrawText(renderdev, 30, 40 + 3 * 8, "Enter Name:", font);
	DrawText(renderdev, 30, 40 + 4 * 8 + 4, " _", font);
	DrawText(renderdev, 30, 40 + 6 * 8, "Enter Callsign:", font);
	// DrawText(renderdev, 30, 40 + 7 * 8 + 4, " _", font);
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
		buttonAtMouseDown = 0;
	}
}

int LoadSaveScreen::GetHotButton()
{
	for (int i = 0; i < 10; i++)
	{
		if (mouseX >= buttonRects[i * 4 + 0] && mouseY >= buttonRects[i * 4 + 1] && mouseX <= buttonRects[i * 4 + 2] && mouseY <= buttonRects[i * 4 + 3])
		{
			return i + 2;
		}
	}
	return 0;
}
