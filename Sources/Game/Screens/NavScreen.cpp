
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
		circle[0] = LoadNavCircleImage(42, 128, 17);
		circle[1] = LoadNavCircleImage(82, 94, 242);
		circle[2] = LoadNavCircleImage(255, 239, 0);
		square[0] = LoadNavSquareImage(42, 128, 17);
		square[1] = LoadNavSquareImage(82, 94, 242);
		square[2] = LoadNavSquareImage(255, 239, 0);
		font = LoadPrivFont(palette.get());
	}

	renderdev->DrawImage(0, 0, background[0].get());

	if (missionMode)
	{
	}
	else
	{
		const WCSector& sector = app->sectordata->sectors[sectorIndex];
		for (const WCSectorSphere& sphere : sector.spheres)
		{
			if (sphere.argtype == 7 && sphere.arg2 == navpoint)
			{
				DrawText(renderdev, 190, 35, "Nav " + std::to_string(navpoint), font, GameTextAlignment::Left, 1);

				if (sphere.arg0 == 1)
				{
					const WCSectorBase& base = app->sectordata->bases[sphere.arg1];
					std::string typetext;
					switch (base.type)
					{
					case WCSectorBaseType::unknown: typetext = " - Unknown"; break;
					case WCSectorBaseType::pleasure: typetext = " - Pleasure"; break;
					case WCSectorBaseType::refinery: typetext = " - Refinery"; break;
					case WCSectorBaseType::mining: typetext = " - Mining"; break;
					case WCSectorBaseType::agricultural: typetext = " - Agricultural"; break;
					case WCSectorBaseType::pirate: typetext = " - Pirate"; break;
					case WCSectorBaseType::special: typetext = " - Special"; break;
					}
					DrawText(renderdev, 190, 45, base.name + typetext, font, GameTextAlignment::Left, 1);
				}
				else if (sphere.arg0 == 2)
				{
					const WCSector& jumpsector = app->sectordata->sectors[sphere.arg1];
					DrawText(renderdev, 190, 45, "Jump to " + jumpsector.label, font, GameTextAlignment::Left, 1);
				}
			}
		}
	}

	renderdev->DrawImage(navmap->wndw.main[0], navmap->wndw.main[1], grid[0].get());

	int64_t halfGridWidth = grid[0]->width / 2;
	int64_t halfGridHeight = grid[0]->height / 2;
	int centerX = navmap->wndw.main[0] + halfGridWidth - 1;
	int centerY = navmap->wndw.main[1] + halfGridHeight - 1;

	if (quadrantMode)
	{
		const WCQuadrant& quad = app->sectordata->quadrants[quadrant];

		DrawText(renderdev, 95, 28, quad.name + " Quadrant", font, GameTextAlignment::Center, 1);

		for (const WCSystem& system : quad.systems)
		{
			int x = centerX + system.x - quad.x;
			int y = centerY + system.y - quad.y;
			renderdev->DrawImage(x, y, circle[0][0].get());
			DrawText(renderdev, x, y + 7, system.name, font, GameTextAlignment::Center, 1);
		}
	}
	else
	{
		const WCSector& sector = app->sectordata->sectors[sectorIndex];

		DrawText(renderdev, 95, 28, "System : " + sector.label, font, GameTextAlignment::Center, 1);

		for (const WCSectorSphere& sphere : sector.spheres)
		{
			if (sphere.argtype == 7)
			{
				int x = centerX + sphere.x * halfGridWidth / 70000;
				int y = centerY - sphere.y * halfGridHeight / 70000;

				int color = 0; // Default to green
				if (sphere.arg0 == 2) // Jump points are blue
					color = 1;
				if (sphere.arg2 == navpoint) // Selected nav point is yellow
					color = 2;

				if (sphere.arg0 == 1)
					renderdev->DrawImage(x, y, square[color][0].get());
				else
					renderdev->DrawImage(x, y, circle[color][0].get());

				DrawText(renderdev, x, y + 7, "Nav " + std::to_string(sphere.arg2), font, GameTextAlignment::Center, 1);
			}
		}
	}

	int index = 0;
	if (buttonAtMouseDown != 0 && buttonAtMouseDown == GetHotButton())
	{
		index = buttonAtMouseDown;
	}

	renderdev->DrawImage(navmap->wndw.button[0], navmap->wndw.button[1], quad[index == 1].get());
	renderdev->DrawImage(navmap->wndw.button[4], navmap->wndw.button[5], navi[index == 2].get());
	renderdev->DrawImage(navmap->wndw.button[8], navmap->wndw.button[9], next[index == 3].get());
}

void NavScreen::OnQuadClick()
{
	quadrantMode = !quadrantMode;
}

void NavScreen::OnMissionClick()
{
	missionMode = !missionMode;
}

void NavScreen::OnNextClick()
{
	if (quadrantMode)
	{
		quadrant = (quadrant + 1) % 4;
	}
	else if (missionMode)
	{
	}
	else
	{
		navpoint++;

		bool foundNavPoint = false;
		const WCSector& sector = app->sectordata->sectors[sectorIndex];
		for (const WCSectorSphere& sphere : sector.spheres)
		{
			if (sphere.argtype == 7 && sphere.arg2 == navpoint)
			{
				foundNavPoint = true;
				break;
			}
		}

		if (!foundNavPoint)
			navpoint = 1;
	}
}

void NavScreen::OnKeyDown(InputKey key)
{
	if (key == InputKey::Escape)
	{
		PopScreen();
	}
	if (key == InputKey::N)
	{
		OnNextClick();
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
			OnQuadClick();
		}
		else if (action == 2)
		{
			OnMissionClick();
		}
		else if (action == 3)
		{
			OnNextClick();
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
