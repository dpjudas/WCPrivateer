#pragma once

#include "GameScreen.h"

class CockpitScreen;

class AutopilotScreen : public GameScreen
{
public:
	AutopilotScreen(GameApp* app, CockpitScreen* cockpit);

	void Render(RenderDevice* renderdev) override;

	CockpitScreen* cockpit = nullptr;
	int64_t StartTime = 0;
	vec3 CameraPos = vec3(0.0f);
	int StartSetSpeed = 0;
};
