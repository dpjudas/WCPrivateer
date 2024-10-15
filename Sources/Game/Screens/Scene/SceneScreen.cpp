
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

		const WCGameFlowMission& baseInfo = app->gamedata->gameFlow.missions[app->playsim.baseIndex];

		// How do we figure out which flow applies?
		flow = nullptr;
		for (const WCGameFlowScene& sceneFlow : baseInfo.scenes)
		{
			if (sceneFlow.sceneIndex == scene)
			{
				if (!flow || flow->targets.size() < sceneFlow.targets.size())
					flow = &sceneFlow;
			}
		}

		startTime = app->GetGameTime();

		palette = LoadPakPalette("DATA\\OPTIONS\\OPTPALS.PAK", app->gamedata->sceneList->scenes[scene].background.palette);
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

	curTime = (int)(app->GetGameTime() - startTime);

	for (size_t i = 0; i < app->gamedata->sceneList->scenes[scene].background.shapes.size(); i++)
	{
		if (!backgrounds[i].empty())
		{
			GameTexture* bgimage = backgrounds[i][(curTime / 150) % backgrounds[i].size()].get();
			const auto& shape = app->gamedata->sceneList->scenes[scene].background.shapes[i];
			renderdev->DrawImage(shape.offsetX, shape.offsetY, bgimage);
		}
	}

	if (!ship.empty())
		renderdev->DrawImage(0, 0, ship[0].get());

	int hotRegion = GetHotRegion();
	if (hotRegion != -1)
	{
		const auto& region = app->gamedata->sceneList->scenes[scene].regions[hotRegion];
		DrawText(renderdev, regionTextX, regionTextY, region.label, font, GameTextAlignment::Center);

		DrawText(renderdev, 10, 20, std::to_string(scene) + " - " + std::to_string((int)region.target), font);
	}
	else
	{
		DrawText(renderdev, 10, 20, std::to_string(scene), font);
	}
}

const WCGameFlowTarget* SceneScreen::GetFlowTarget(WCTarget target)
{
	if (flow)
	{
		for (const WCGameFlowTarget& t : flow->targets)
		{
			if (t.target == (int)target)
			{
				return &t;
			}
		}
	}
	return nullptr;
}

void SceneScreen::SetScene(const WCGameFlowTarget* target)
{
	if (!target || target->effect != WCGameFlowEffect::SetScene)
		return;

	nextScene = target->args[0];
}

void SceneScreen::DrawSprite(RenderDevice* renderdev, int i, int x, int y)
{
	if (i < 0 || (size_t)i >= sprites.size() || sprites[i].empty())
		return;

	const auto& sprite = app->gamedata->sceneList->scenes[scene].foreground.sprites[i];
	int frame = sprite.sequence.empty() ? (curTime / 150) % sprites[i].size() : sprite.sequence[(curTime / 150) % sprite.sequence.size()];
	GameTexture* fgimage = sprites[i][frame].get();
	renderdev->DrawImage(x + sprite.x1, y + sprite.y1, fgimage);
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
