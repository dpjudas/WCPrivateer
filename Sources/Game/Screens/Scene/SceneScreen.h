#pragma once

#include "Game/Screens/GameScreen.h"
#include "FileFormat/WCScene.h"

class WCSceneList;

class SceneScreen : public GameScreen
{
public:
	SceneScreen(GameApp* app);
	~SceneScreen();

	void OnKeyDown(InputKey key) override;
	void OnKeyUp(InputKey key) override;
	void Render(RenderDevice* renderdev) override;

	void DrawSprite(RenderDevice* renderdev, int index, int x = 0, int y = 0);

	virtual void OnClickTarget(WCTarget target) { }

	int GetHotRegion();

	std::vector<std::vector<std::unique_ptr<GameTexture>>> backgrounds;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> sprites;
	std::vector<std::unique_ptr<GameTexture>> ship;
	std::vector<std::unique_ptr<GameTexture>> font;

	int scene = -1;
	int nextScene = 0;

	int framecounter = 0;
};
