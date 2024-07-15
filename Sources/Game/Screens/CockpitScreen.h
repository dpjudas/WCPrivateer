#pragma once

#include "GameScreen.h"

class WCCockpit;
class WCCockpitSoftware;
class WCCockpitMisc;
class WCCockpitPlaques;
class WCSpaceSprite;
class WCSpaceShip;

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
	std::unique_ptr<WCCockpitSoftware> software;
	std::unique_ptr<WCCockpitMisc> misc;
	std::unique_ptr<WCCockpitPlaques> plaques;

	std::unique_ptr<WCSpaceSprite> trash[4];
	std::unique_ptr<WCSpaceSprite> astroids[2];
	std::unique_ptr<WCSpaceSprite> starwhite;
	std::unique_ptr<WCSpaceSprite> moon[3];
	std::unique_ptr<WCSpaceSprite> refine;
	std::unique_ptr<WCSpaceShip> frigate;

	std::unique_ptr<WCPalette> palette;
	std::vector<std::unique_ptr<GameTexture>> front;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> sprites;
	std::vector<std::unique_ptr<GameTexture>> refineTex;
	std::vector<std::vector<std::unique_ptr<GameTexture>>> frigateTex;
	std::vector<std::unique_ptr<GameTexture>> frigateTarget;
	int framecounter = 0;

	std::vector<std::unique_ptr<GameTexture>> crosshair;
	std::vector<std::unique_ptr<GameTexture>> navigationCrosshair;
	std::vector<std::unique_ptr<GameTexture>> itts;
	std::vector<std::unique_ptr<GameTexture>> shield;
	std::vector<std::unique_ptr<GameTexture>> fuel;
	std::vector<std::unique_ptr<GameTexture>> autopilot;
	std::vector<std::unique_ptr<GameTexture>> energy;

	std::vector<std::unique_ptr<GameTexture>> font;

	std::vector<std::unique_ptr<GameTexture>> guns;
	std::vector<std::unique_ptr<GameTexture>> weapons;

	std::vector<std::unique_ptr<GameTexture>> explosion;
	std::vector<std::unique_ptr<GameTexture>> plaquetextures;

	std::unique_ptr<GameTexture> blackTexture;
};
