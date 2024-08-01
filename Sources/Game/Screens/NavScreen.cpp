
#include "NavScreen.h"
#include "Game/GameApp.h"

NavScreen::NavScreen(GameApp* app) : GameScreen(app)
{
	navmap = std::make_unique<WCNavMapType>(app->archive.get());
}

void NavScreen::Render(RenderDevice* renderdev)
{
	if (background.empty())
	{
		palette = LoadSpacePalette();
		background = LoadWCImage(*navmap->background, palette.get());
		grid = LoadWCImage(*navmap->grid, palette.get());
		mouse = LoadWCImage(*navmap->mouse, palette.get());
		quad = LoadWCImage(*navmap->button.quad, palette.get());
		navi = LoadWCImage(*navmap->button.navi, palette.get());
		next = LoadWCImage(*navmap->button.next, palette.get());
	}

	renderdev->DrawImage(0, 0, background[0].get());
	renderdev->DrawImage(navmap->wndw.main[0], navmap->wndw.main[1], grid[0].get());

	int index = 0;
	if (buttonAtMouseDown != 0 && buttonAtMouseDown == GetHotButton())
	{
		index = buttonAtMouseDown;
	}

	renderdev->DrawImage(navmap->wndw.button[0], navmap->wndw.button[1], quad[index == 1].get());
	renderdev->DrawImage(navmap->wndw.button[4], navmap->wndw.button[5], navi[index == 2].get());
	renderdev->DrawImage(navmap->wndw.button[8], navmap->wndw.button[9], next[index == 3].get());
}

void NavScreen::OnKeyDown(InputKey key)
{
	if (key == InputKey::Escape)
	{
		PopScreen();
	}
	else if (key == InputKey::LeftMouse)
	{
		buttonAtMouseDown = GetHotButton();
	}
}

void NavScreen::OnKeyUp(InputKey key)
{
	if (key == InputKey::LeftMouse)
	{
		buttonAtMouseDown = 0;
		int action = GetHotButton();
		if (action == 1)
		{
			// Toggle quadrant/system mode
		}
		else if (action == 2)
		{
			// Toggle navpoints/missions mode
		}
		else if (action == 3)
		{
			// Next nav point
		}
	}
}

int NavScreen::GetHotButton()
{
	if (mouseX >= navmap->wndw.button[0] && mouseY >= navmap->wndw.button[1] && mouseX < navmap->wndw.button[2] && mouseY < navmap->wndw.button[3])
	{
		return 1;
	}
	else if (mouseX >= navmap->wndw.button[4] && mouseY >= navmap->wndw.button[5] && mouseX < navmap->wndw.button[6] && mouseY < navmap->wndw.button[7])
	{
		return 2;
	}
	else if (mouseX >= navmap->wndw.button[8] && mouseY >= navmap->wndw.button[9] && mouseX < navmap->wndw.button[10] && mouseY < navmap->wndw.button[11])
	{
		return 3;
	}
	return 0;
}
