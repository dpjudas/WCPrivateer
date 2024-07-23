#pragma once

#include "MovieScreen.h"

class WCMovie;
class WCVOCSound;

class IntroScene
{
public:
	std::vector<std::vector<std::unique_ptr<GameTexture>>> images;
	std::vector<int> x;
	std::vector<int> y;
};

class IntroScreen : public GameScreen
{
public:
	IntroScreen(GameApp* app);
	~IntroScreen();

	void AddScene(int start, int count, int palindex, std::string imagepak);
	void AddText(int start, int count, std::string textpak);
	void AddPirateScene(std::string imagepak, std::string fontname);
	void AddProtagonistScene(std::string imagepak, std::string fontname);

	void Render(RenderDevice* renderdev) override;
	void OnKeyDown(InputKey key) override;

	void RenderMovieScenes(RenderDevice* renderdev, int nextScene);
	void RenderConversationScene(RenderDevice* renderdev, int nextScene);

	std::unique_ptr<WCMovie> movie;
	std::vector<std::unique_ptr<GameTexture>> font;
	std::vector<IntroScene> scenes;
	std::vector<std::unique_ptr<WCVOCSound>> sounds;
	std::vector<std::string> scenetext;

	std::vector<std::unique_ptr<GameTexture>> background;
	std::vector<std::unique_ptr<GameTexture>> face;
	int faceX = 0;
	int faceY = 0;

	void SetStartTime();
	int64_t GetMovieTime();
	int64_t StartTimeNS = 0;

	void SetScene(int index);
	int currentScene = 0;
	int currentSound = -1;
};
