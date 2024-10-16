#pragma once

#include "Game/Screens/GameScreen.h"
#include "FileFormat/WCScene.h"
#include "FileFormat/WCGameData.h"

class WCSceneList;
class WCGameFlowScene;
class WCGameFlowTarget;

class SceneScreen : public GameScreen
{
public:
	SceneScreen(GameApp* app);
	~SceneScreen();

	void OnKeyDown(InputKey key) override;
	void OnKeyUp(InputKey key) override;
	void Render(RenderDevice* renderdev) override;

	void DrawSprite(RenderDevice* renderdev, int index, int x = 0, int y = 0);

	void SetScene(const WCGameFlowTarget* target);
	const WCGameFlowTarget* GetFlowTarget(WCTarget target);

	virtual void OnClickTarget(WCTarget target);

	int GetHotRegion();
	int GetHotSprite();

	std::unique_ptr<WCPalette> palette;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> backgrounds;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> sprites;
	std::vector<std::unique_ptr<GameTexture>> ship;
	std::vector<std::unique_ptr<GameTexture>> font;

	int scene = -1;
	int nextScene = -1;
	const WCGameFlowScene* flow = nullptr;

	int64_t startTime = 0;
	int curTime = 0;

	int regionTextX = 160;
	int regionTextY = 190;
};
