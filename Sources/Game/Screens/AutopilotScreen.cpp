
#include "AutopilotScreen.h"
#include "CockpitScreen.h"
#include "Game/GameApp.h"
#include "Game/GameObjects/GameObject.h"

AutopilotScreen::AutopilotScreen(GameApp* app, CockpitScreen* cockpit) : GameScreen(app), cockpit(cockpit)
{
	StartTime = app->GetGameTime();
	CameraPos = app->playsim.player->position + app->playsim.player->rotation * vec3(200.0f, 0.0f, 100.0f);
	StartSetSpeed = app->playsim.player->setSpeed;
	app->playsim.player->setSpeed = 300;
	app->playsim.player->velocity = app->playsim.player->rotation * vec3(0.0f, 0.0f, 400.0f);
}

void AutopilotScreen::Render(RenderDevice* renderdev)
{
	quaternion rotation = rotation_between(vec3(0.0f, 0.0f, 1.0f), normalize(app->playsim.player->rotation * (app->playsim.player->position - CameraPos)));
	quaternion cameraRotation = app->playsim.player->rotation * rotation;

	cockpit->TickGameObjects();
	cockpit->RenderViewport(renderdev, CameraPos, cameraRotation, false);

	int64_t curTime = app->GetGameTime();
	int64_t timeElapsed = curTime - StartTime;
	if (timeElapsed >= 10'000)
	{
		app->playsim.player->setSpeed = StartSetSpeed;
		cockpit->FlyToNavpoint();
		PopScreen();
	}
}
