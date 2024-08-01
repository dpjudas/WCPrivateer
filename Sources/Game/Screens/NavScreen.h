#pragma once

#include "GameScreen.h"
#include "FileFormat/WCTypes.h"

class NavScreen : public GameScreen
{
public:
	NavScreen(GameApp* app);

	void Render(RenderDevice* renderdev) override;
	void OnKeyDown(InputKey key) override;
	void OnKeyUp(InputKey key) override;

	int GetHotButton();

	std::unique_ptr<WCNavMapType> navmap;

	int buttonAtMouseDown = 0;

	std::unique_ptr<WCPalette> palette;
	std::vector<std::unique_ptr<GameTexture>> background;
	std::vector<std::unique_ptr<GameTexture>> grid;
	std::vector<std::unique_ptr<GameTexture>> mouse;
	std::vector<std::unique_ptr<GameTexture>> quad;
	std::vector<std::unique_ptr<GameTexture>> navi;
	std::vector<std::unique_ptr<GameTexture>> next;
};
