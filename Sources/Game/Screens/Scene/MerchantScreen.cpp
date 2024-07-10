
#include "MerchantScreen.h"
#include "BaseScreen.h"

MerchantScreen::MerchantScreen(GameApp* app) : SceneScreen(app)
{
	nextScene = 61;
}

void MerchantScreen::Render(RenderDevice* renderdev)
{
	// pakindex for screen fade effect is 17
	// pakindex for cds is 22

	SceneScreen::Render(renderdev);
	DrawSprite(renderdev, 0);
}

void MerchantScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<BaseScreen>(app));
	}
	else if (target == WCTarget::Computer)
	{
		nextScene = 62;
	}
}
