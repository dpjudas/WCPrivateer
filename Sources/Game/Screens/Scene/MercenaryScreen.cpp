
#include "MercenaryScreen.h"
#include "BaseScreen.h"

MercenaryScreen::MercenaryScreen(GameApp* app, const WCGameFlowTarget* target) : SceneScreen(app)
{
	SetScene(target);
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
		ShowScreen(std::make_unique<BaseScreen>(app, GetFlowTarget(target)));
	}
	else if (target == WCTarget::Computer)
	{
		nextScene = 60;
	}
}
