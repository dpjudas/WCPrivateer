
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
		ship.reset();
		scene = nextScene;

		auto palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", sceneList->scenes[scene].background.palette);
		for (const auto& bgshape : sceneList->scenes[scene].background.shapes)
		{
			try
			{
				backgrounds.push_back(LoadPakImage("DATA\\OPTIONS\\OPTSHPS.PAK", bgshape.optpakIndex, palette.get()));
			}
			catch (...)
			{
				// To do: figure out why some images fail to load
				backgrounds.push_back(nullptr);
			}
		}
		if (sceneList->scenes[scene].ship.shape)
			ship = LoadWCImage(*sceneList->scenes[scene].ship.shape, 0);
	}

	for (size_t i = 0; i < sceneList->scenes[scene].background.shapes.size(); i++)
	{
		if (backgrounds[i])
		{
			renderdev->DrawImage(backgrounds[i]->x + sceneList->scenes[scene].background.shapes[i].offsetX, backgrounds[i]->y + sceneList->scenes[scene].background.shapes[i].offsetY, backgrounds[i]->width, backgrounds[i]->height, backgrounds[i].get());
		}
	}

	if (ship)
		renderdev->DrawImage(ship->x, ship->y, ship->width, ship->height, ship.get());
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
