
#include "MercenaryScreen.h"
#include "BaseScreen.h"

MercenaryScreen::MercenaryScreen(GameApp* app) : SceneScreen(app)
{
	nextScene = 59;
}

void MercenaryScreen::Render(RenderDevice* renderdev)
{
	SceneScreen::Render(renderdev);
	DrawSprite(renderdev, 0);
}

void MercenaryScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<BaseScreen>(app));
	}
	else if (target == WCTarget::Computer)
	{
		nextScene = 60;
	}
}
