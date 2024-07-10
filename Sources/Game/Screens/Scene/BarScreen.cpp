
#include "BarScreen.h"
#include "BaseScreen.h"

BarScreen::BarScreen(GameApp* app) : SceneScreen(app)
{
	nextScene = 5;
}

void BarScreen::Render(RenderDevice* renderdev)
{
	SceneScreen::Render(renderdev);
	DrawSprite(renderdev, 0);
}

void BarScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<BaseScreen>(app));
	}
}
