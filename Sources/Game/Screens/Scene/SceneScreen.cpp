
#include "Precomp.h"
#include "SceneScreen.h"
#include "FileFormat/WCScene.h"
#include "Game/GameApp.h"

SceneScreen::SceneScreen(GameApp* app) : GameScreen(app)
{
	sceneList = std::make_unique<WCSceneList>(app->archive.get());
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

		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", sceneList->scenes[scene].background.palette);
		for (const auto& bgshape : sceneList->scenes[scene].background.shapes)
		{
			backgrounds.push_back(LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", bgshape.optpakIndex, palette.get()));
		}

		for (const auto& fgsprite : sceneList->scenes[scene].foreground.sprites)
		{
			sprites.push_back(LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", fgsprite.optpakIndex, palette.get()));
		}

		if (sceneList->scenes[scene].ship.shape)
			ship = LoadWCImage(*sceneList->scenes[scene].ship.shape);

		// DATA\\OPTIONS\\FONTS.IFF (seems to also be a shape with glyphs, but placed in an IFF header)
		//font = LoadShpImage("DATA\\FONTS\\CONVFONT.SHP", palette.get());
		//font = LoadShpImage("DATA\\FONTS\\DEMOFONT.SHP", palette.get());
		//font = LoadShpImage("DATA\\FONTS\\MSSGFONT.SHP", palette.get());
		//font = LoadShpImage("DATA\\FONTS\\PCFONT.SHP", palette.get());
		//font = LoadShpImage("DATA\\FONTS\\PRIVFNT.SHP", palette.get());

		font = LoadShpImage("DATA\\FONTS\\OPTFONT.SHP", palette.get());
	}

	static int framecounter = 0;
	framecounter++;

	for (size_t i = 0; i < sceneList->scenes[scene].background.shapes.size(); i++)
	{
		if (!backgrounds[i].empty())
		{
			GameTexture* bgimage = backgrounds[i][(framecounter / 20) % backgrounds[i].size()].get();
			const auto& shape = sceneList->scenes[scene].background.shapes[i];
			renderdev->DrawImage(bgimage->x + shape.offsetX, bgimage->y + shape.offsetY, bgimage->width, bgimage->height, bgimage);
		}
	}

	for (size_t i = 0; i < sceneList->scenes[scene].foreground.sprites.size(); i++)
	{
		if (!sprites[i].empty())
		{
			const auto& sprite = sceneList->scenes[scene].foreground.sprites[i];
			int frame = sprite.sequence.empty() ? (framecounter / 20) % sprites[i].size() : sprite.sequence[(framecounter / 20) % sprite.sequence.size()];
			GameTexture* fgimage = sprites[i][frame].get();
			renderdev->DrawImage(fgimage->x + sprite.x1, fgimage->y + sprite.y1, fgimage->width, fgimage->height, fgimage);

			break; // Only show one of the actors
		}
	}

	if (!ship.empty())
		renderdev->DrawImage(ship[0]->x, ship[0]->y, ship[0]->width, ship[0]->height, ship[0].get());

	for (auto& region : sceneList->scenes[scene].regions)
	{
		// Use a crude bounding box check. Probably precise enough.
		if (region.coords.empty())
			continue;
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
			continue;

		std::string text = region.label;
		int textwidth = GetTextWidth(region.label, font);
		int x = (320 - textwidth) / 2;
		int y = 190;
		DrawText(renderdev, x, y, text, font);

		break;
	}
}

void SceneScreen::OnKeyDown(InputKey key)
{
	if (key == InputKey::Left)
	{
		nextScene = std::max(scene - 1, 0);
	}
	else if (key == InputKey::Right)
	{
		nextScene = std::min(scene + 1, (int)sceneList->scenes.size() - 1);
	}
}
