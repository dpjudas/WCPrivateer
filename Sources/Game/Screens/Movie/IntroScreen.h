#pragma once

#include "MovieScreen.h"

class WCMovie;
class WCVOCSound;

class IntroScreen : public GameScreen
{
public:
	IntroScreen(GameApp* app);
	~IntroScreen();

	void Render(RenderDevice* renderdev) override;
	void OnKeyDown(InputKey key) override;

	std::unique_ptr<WCMovie> movie;
	std::unique_ptr<WCPalette> palette;
	std::vector<std::unique_ptr<GameTexture>> font;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> scene;
	std::vector<std::unique_ptr<WCVOCSound>> sounds;
	std::vector<std::string> scenetext;

	void SetStartTime();
	int64_t GetMovieTime();
	int64_t StartTimeNS = 0;

	void SetScene(int index);
	int currentScene = 0;
	int currentSound = -1;
};
