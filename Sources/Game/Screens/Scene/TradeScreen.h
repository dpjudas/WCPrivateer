#pragma once

#include "SceneScreen.h"

class TradeScreen : public SceneScreen
{
public:
	TradeScreen(GameApp* app, const WCGameFlowTarget* target);
	void Render(RenderDevice* renderdev) override;
	void OnClickTarget(WCTarget target) override;

	std::vector<std::unique_ptr<GameTexture>> panel;
	std::vector<std::unique_ptr<GameTexture>> buttons;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> items;
	std::vector<std::unique_ptr<GameTexture>> smallFont;

	int mode = 1;
	int selectedItem = 0;
};
