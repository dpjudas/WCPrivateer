#pragma once

#include "GameScreen.h"

class MainMenuScreen : public GameScreen
{
public:
	MainMenuScreen(GameApp* app);
	~MainMenuScreen();

	void OnKeyDown(InputKey key) override;
	void OnKeyUp(InputKey key) override;
	void Render(RenderDevice* renderdev) override;

	int GetHotButton();

	std::vector<std::unique_ptr<GameTexture>> background;
	std::vector<std::unique_ptr<GameTexture>> buttons;

	int buttonAtMouseDown = 0;

	int buttonTop = 182;
	int buttonBottom = 193;
	int newLeft = 8;
	int newRight = 83;
	int loadLeft = 85;
	int loadRight = 159;
	int optionsLeft = 161;
	int optionsRight = 235;
	int quitLeft = 237;
	int quitRight = 311;
};
