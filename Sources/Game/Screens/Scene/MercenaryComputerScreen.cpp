
#include "MercenaryComputerScreen.h"

MercenaryComputerScreen::MercenaryComputerScreen(GameApp* app, const WCGameFlowTarget* target) : SceneScreen(app)
{
	nextScene = 60;
}

void MercenaryComputerScreen::Render(RenderDevice* renderdev)
{
	SceneScreen::Render(renderdev);
	DrawSprite(renderdev, 0);
}

void MercenaryComputerScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		PopScreen();
	}
}
