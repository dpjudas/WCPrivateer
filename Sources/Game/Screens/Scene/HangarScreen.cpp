
#include "HangarScreen.h"
#include "BaseScreen.h"

HangarScreen::HangarScreen(GameApp* app) : SceneScreen(app)
{
	nextScene = 44;
}

void HangarScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<BaseScreen>(app));
	}
}
