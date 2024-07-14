#pragma once

#include "GameScreen.h"

class WCCockpit;
class WCSpaceSprite;

struct StarLocation
{
	int x = 0;
	int y = 0;
	int index = 0;
};

struct SpriteLocation
{
	int x = 0;
	int y = 0;
	int index = 0;
	float scale = 1.0f;
};

class CockpitScreen : public GameScreen
{
public:
	CockpitScreen(GameApp* app);
	~CockpitScreen();

	void Render(RenderDevice* renderdev) override;

	std::vector<std::vector<std::unique_ptr<GameTexture>>> stars;

	std::vector<StarLocation> starLocations;
	std::vector<SpriteLocation> spriteLocations;

	std::unique_ptr<WCCockpit> cockpit;
	std::unique_ptr<WCSpaceSprite> trash[4];
	std::unique_ptr<WCSpaceSprite> astroids[2];
	std::unique_ptr<WCSpaceSprite> starwhite;
	std::unique_ptr<WCSpaceSprite> moon[3];

	std::unique_ptr<WCPalette> palette;
	std::vector<std::unique_ptr<GameTexture>> front;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> sprites;
	int framecounter = 0;

	std::vector<std::unique_ptr<GameTexture>> crosshair;
	std::vector<std::unique_ptr<GameTexture>> navigationCrosshair;
	std::vector<std::unique_ptr<GameTexture>> itts;
	std::vector<std::unique_ptr<GameTexture>> shield;
	std::vector<std::unique_ptr<GameTexture>> fuel;
	std::vector<std::unique_ptr<GameTexture>> autopilot;
	std::vector<std::unique_ptr<GameTexture>> energy;

	std::vector<std::unique_ptr<GameTexture>> font;

	std::unique_ptr<GameTexture> blackTexture;
};
