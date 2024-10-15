
#include "HangarScreen.h"
#include "BaseScreen.h"
#include "Game/Screens/Movie/TakeoffScreen.h"
#include "Game/GameApp.h"

HangarScreen::HangarScreen(GameApp* app) : SceneScreen(app)
{
	const WCGameFlowMission& baseInfo = app->gamedata->gameFlow.missions[app->playsim.baseIndex];

	static bool firstcall = true;
	if (firstcall)
	{
		PlayMusic("DATA\\SOUND\\BASETUNE.GEN", baseInfo.tune);
		firstcall = false;
	}

	nextScene = baseInfo.startSceneIndex;
}

void HangarScreen::OnClickTarget(WCTarget target)
{
	if (target == WCTarget::MainConcourse)
	{
		ShowScreen(std::make_unique<BaseScreen>(app, GetFlowTarget(target)));
	}
	else if (target == WCTarget::Launch)
	{
		ShowScreen(std::make_unique<TakeoffScreen>(app));
	}
}
