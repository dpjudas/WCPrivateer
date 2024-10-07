#pragma once

#include "GameScreen.h"
#include "FileFormat/WCTypes.h"
#include "Math/Vec.h"

class WCCockpit;
class WCCockpitSoftware;
class WCCockpitMisc;
class WCCockpitPlaques;
class WCSpaceSprite;
class WCSpaceShip;

struct StarLocation
{
	vec3 position = vec3(0.0f);
	int index = 0;
};

class CockpitScreen : public GameScreen
{
public:
	CockpitScreen(GameApp* app);
	~CockpitScreen();

	void Render(RenderDevice* renderdev) override;
	void OnKeyDown(InputKey key) override;
	void OnKeyUp(InputKey key) override;

	std::unique_ptr<WCTargetingType> targeting;

	std::vector<std::vector<std::unique_ptr<GameTexture>>> stars;

	std::vector<StarLocation> starLocations;

	std::unique_ptr<WCCockpit> cockpit;
	std::unique_ptr<WCCockpitSoftware> software;
	std::unique_ptr<WCCockpitMisc> misc;
	std::unique_ptr<WCCockpitPlaques> plaques;

	struct Sprite
	{
		std::unique_ptr<WCSpaceSprite> sprite;
		std::vector<std::unique_ptr<GameTexture>> shape;
		std::vector<std::unique_ptr<GameTexture>> target;
	};

	struct Ship
	{
		std::unique_ptr<WCSpaceShip> ship;
		std::vector<std::vector<std::unique_ptr<GameTexture>>> shapes;
		std::vector<std::unique_ptr<GameTexture>> target;
		std::vector<std::unique_ptr<GameTexture>> weapon;
		std::vector<std::unique_ptr<GameTexture>> burn;
	};

	std::map<std::string, Sprite> spaceSprites;
	std::map<std::string, Ship> spaceShips;

	Sprite* getSprite(const std::string& name);
	Ship* getShip(const std::string& name);

	std::unique_ptr<WCSpaceSprite> starwhite;
	std::unique_ptr<WCSpaceSprite> moon[3];

	std::unique_ptr<WCPalette> palette;
	std::vector<std::unique_ptr<GameTexture>> front;

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

	std::vector<std::unique_ptr<GameTexture>> radar;

	std::unique_ptr<GameTexture> blackTexture;
};
