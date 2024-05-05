#pragma once

#include "Game/Screens/GameScreen.h"

class WCSceneList;

class SceneScreen : public GameScreen
{
public:
	SceneScreen(GameApp* app);
	~SceneScreen();

	void OnKeyDown(InputKey key) override;
	void Render(RenderDevice* renderdev) override;

	std::unique_ptr<WCSceneList> sceneList;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> backgrounds;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> sprites;
	std::vector<std::unique_ptr<GameTexture>> ship;
	std::vector<std::unique_ptr<GameTexture>> font;

	int scene = -1;
	int nextScene = 0;
};
