
#include "Precomp.h"
#include "SceneScreen.h"
#include "FileFormat/WCScene.h"
#include "Game/GameApp.h"

SceneScreen::SceneScreen(GameApp* app) : GameScreen(app)
{
}

SceneScreen::~SceneScreen()
{
}

void SceneScreen::Render(RenderDevice* renderdev)
{
	if (scene != nextScene)
	{
		backgrounds.clear();
		sprites.clear();
		ship.clear();
		scene = nextScene;

		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", app->gamedata->sceneList->scenes[scene].background.palette);
		for (const auto& bgshape : app->gamedata->sceneList->scenes[scene].background.shapes)
		{
			backgrounds.push_back(LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", bgshape.optpakIndex, palette.get()));
		}

		for (const auto& fgsprite : app->gamedata->sceneList->scenes[scene].foreground.sprites)
		{
			sprites.push_back(LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", fgsprite.optpakIndex, palette.get()));
		}

		if (app->gamedata->sceneList->scenes[scene].ship.shape)
			ship = LoadWCImage(*app->gamedata->sceneList->scenes[scene].ship.shape, palette.get());

		font = LoadOptFont(palette.get());
	}

	framecounter++;

	for (size_t i = 0; i < app->gamedata->sceneList->scenes[scene].background.shapes.size(); i++)
	{
		if (!backgrounds[i].empty())
		{
			GameTexture* bgimage = backgrounds[i][(framecounter / 20) % backgrounds[i].size()].get();
			const auto& shape = app->gamedata->sceneList->scenes[scene].background.shapes[i];
			renderdev->DrawImage(bgimage->x + shape.offsetX, bgimage->y + shape.offsetY, bgimage->width, bgimage->height, bgimage);
		}
	}

	if (!ship.empty())
		renderdev->DrawImage(ship[0]->x, ship[0]->y, ship[0]->width, ship[0]->height, ship[0].get());

	int hotRegion = GetHotRegion();
	if (hotRegion != -1)
	{
		const auto& region = app->gamedata->sceneList->scenes[scene].regions[hotRegion];

		std::string text = region.label + " - " + std::to_string((int)region.target);
		int textwidth = GetTextWidth(region.label, font);
		int x = (320 - textwidth) / 2;
		int y = 190;
		DrawText(renderdev, x, y, text, font);
	}

	DrawText(renderdev, 10, 20, std::to_string(scene), font);
}

void SceneScreen::DrawSprite(RenderDevice* renderdev, int i, int x, int y)
{
	if (i < 0 || (size_t)i >= sprites.size() || sprites[i].empty())
		return;

	const auto& sprite = app->gamedata->sceneList->scenes[scene].foreground.sprites[i];
	int frame = sprite.sequence.empty() ? (framecounter / 20) % sprites[i].size() : sprite.sequence[(framecounter / 20) % sprite.sequence.size()];
	GameTexture* fgimage = sprites[i][frame].get();
	renderdev->DrawImage(x + fgimage->x + sprite.x1, y + fgimage->y + sprite.y1, fgimage->width, fgimage->height, fgimage);
}

int SceneScreen::GetHotRegion()
{
	int index = 0;
	for (auto& region : app->gamedata->sceneList->scenes[scene].regions)
	{
		if (region.coords.empty())
		{
			index++;
			continue;
		}

		// Use a crude bounding box check. Probably precise enough.
		int minx = region.coords.front().x;
		int miny = region.coords.front().y;
		int maxx = region.coords.front().x;
		int maxy = region.coords.front().y;
		for (const auto& c : region.coords)
		{
			minx = std::min(minx, c.x);
			miny = std::min(miny, c.y);
			maxx = std::max(maxx, c.x);
			maxy = std::max(maxy, c.y);
		}

		if (mouseX < minx || mouseX > maxx || mouseY < miny || mouseY > maxy)
		{
			index++;
			continue;
		}

		return index;
	}

	return -1;
}

void SceneScreen::OnKeyDown(InputKey key)
{
	if (key == InputKey::Left)
	{
		nextScene = std::max(scene - 1, 0);
	}
	else if (key == InputKey::Right)
	{
		nextScene = std::min(scene + 1, (int)app->gamedata->sceneList->scenes.size() - 1);
	}
}

void SceneScreen::OnKeyUp(InputKey key)
{
	if (key == InputKey::LeftMouse)
	{
		int hotRegion = GetHotRegion();
		if (hotRegion != -1)
		{
			const auto& region = app->gamedata->sceneList->scenes[scene].regions[hotRegion];
			OnClickTarget(region.target);
		}
	}
}
