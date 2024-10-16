
#include "MerchantComputerScreen.h"

MerchantComputerScreen::MerchantComputerScreen(GameApp* app, const WCGameFlowTarget* target) : SceneScreen(app)
{
	nextScene = 62;
}

void MerchantComputerScreen::Render(RenderDevice* renderdev)
{
	// pakindex for screen fade effect is 17
	// pakindex for cds is 22

	SceneScreen::Render(renderdev);
	DrawSprite(renderdev, 0);
}

void MerchantComputerScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		PopScreen();
	}
}
