
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
		ship.clear();
		scene = nextScene;

		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", sceneList->scenes[scene].background.palette);
		for (const auto& bgshape : sceneList->scenes[scene].background.shapes)
		{
			backgrounds.push_back(LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", bgshape.optpakIndex, palette.get()));
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

	if (!ship.empty())
		renderdev->DrawImage(ship[0]->x, ship[0]->y, ship[0]->width, ship[0]->height, ship[0].get());

	if (!sceneList->scenes[scene].regions.empty())
	{
		std::string text = sceneList->scenes[scene].regions[0].label;
		int textwidth = 0;
		for (char c : text)
		{
			uint8_t i = c;
			if (i == 32 && font.size() > 'x')
			{
				textwidth += font['x']->width + 1;
			}
			else if (i < font.size() && !font[i]->pixels.empty())
			{
				textwidth += font[i]->width - font[i]->x + 1;
			}
		}

		int x = (320 - textwidth) / 2;
		int y = 190;
		for (char c : text)
		{
			uint8_t i = c;
			if (i == 32 && font.size() > 'x')
			{
				x += font['x']->width + 1;
			}
			else if (i < font.size() && !font[i]->pixels.empty())
			{
				renderdev->DrawImage(font[i]->x + x, font[i]->y + y, font[i]->width, font[i]->height, font[i].get());
				x += font[i]->width - font[i]->x + 1;
			}
		}
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
