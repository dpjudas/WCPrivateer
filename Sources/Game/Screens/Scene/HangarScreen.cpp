
#include "HangarScreen.h"
#include "BaseScreen.h"
#include "Game/Screens/Movie/TakeoffScreen.h"

HangarScreen::HangarScreen(GameApp* app) : SceneScreen(app)
{
	static bool firstcall = true;
	if (firstcall)
	{
		PlayMusic("DATA\\SOUND\\BASETUNE.GEN", 8);
		firstcall = false;
	}

	nextScene = 44;
}

void HangarScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<BaseScreen>(app));
	}
	else if (target == WCTarget::Launch)
	{
		ShowScreen(std::make_unique<TakeoffScreen>(app));
	}
}
