#pragma once

#include "SceneScreen.h"

class MissionComputerScreen : public SceneScreen
{
public:
	MissionComputerScreen(GameApp* app);
	void Render(RenderDevice* renderdev) override;
	void OnClickTarget(WCTarget target) override;

	std::string FormatText(std::string format, ...);
	void ReplaceString(std::string& text, std::string search, std::string replace);

	std::unique_ptr<GameTexture> computerscreen;
	std::vector<std::unique_ptr<GameTexture>> smallFont;
	std::string screenText;
	GameTextAlignment screenAlign = GameTextAlignment::Center;
};
