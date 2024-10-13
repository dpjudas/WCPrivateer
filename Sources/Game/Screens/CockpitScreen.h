#pragma once

#include "GameScreen.h"
#include "FileFormat/WCTypes.h"
#include "Math/Vec.h"
#include "Math/Quaternion.h"

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

enum class CockpitConsoleMode
{
	target,
	view,
	destination,
	comm,
	damageReport,
	weapons,
	cargoManifest
};

class CockpitScreen : public GameScreen
{
public:
	CockpitScreen(GameApp* app);
	~CockpitScreen();

	void Render(RenderDevice* renderdev) override;
	void OnKeyDown(InputKey key) override;
	void OnKeyUp(InputKey key) override;
	void ReleaseKeys();

	void FlyToNavpoint();
	void TickGameObjects();
	void CheckResources();
	void FindClosestTarget();
	void RenderViewport(RenderDevice* renderdev, const vec3& viewPos, const quaternion& viewRotation, bool hidePlayer = true);
	void DrawTargetLock(RenderDevice* renderdev);
	void DrawNavPoint(RenderDevice* renderdev);
	void DrawEnergyIndicator(RenderDevice* renderdev);
	void DrawFuelIndicator(RenderDevice* renderdev);
	void DrawAutopilotIndicator(RenderDevice* renderdev);
	void DrawShieldIndicator(RenderDevice* renderdev);
	void DrawRadar(RenderDevice* renderdev);
	void DrawConsole(RenderDevice* renderdev);
	void DrawConsoleModeTarget(RenderDevice* renderdev, int x, int y);
	void DrawConsoleModeView(RenderDevice* renderdev, int x, int y);
	void DrawConsoleModeDestination(RenderDevice* renderdev, int x, int y);
	void DrawConsoleModeComm(RenderDevice* renderdev, int x, int y);
	void DrawConsoleModeDamageReport(RenderDevice* renderdev, int x, int y);
	void DrawConsoleModeWeapons(RenderDevice* renderdev, int x, int y);
	void DrawConsoleModeCargoManifest(RenderDevice* renderdev, int x, int y);
	void DrawSpeed(RenderDevice* renderdev);
	void DrawCrosshair(RenderDevice* renderdev);
	void DrawCockpit(RenderDevice* renderdev);
	void DrawPauseDialog(RenderDevice* renderdev);

	static ivec2 GetRadarPos(const vec3& objPos, const vec3& viewPos, const quaternion& viewRotation, int radarX, int radarY, int radarSize);

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
	std::vector<std::unique_ptr<GameTexture>> plaquesmallfont;
	std::map<std::string, std::vector<std::unique_ptr<GameTexture>>> plaquefonts;

	std::vector<std::unique_ptr<GameTexture>> radar;
	std::vector<std::unique_ptr<GameTexture>> dots[4];

	std::unique_ptr<GameTexture> blackTexture;
	std::unique_ptr<GameTexture> whiteTexture;

	vec2 targetBoxTL = vec2(0.0f);
	vec2 targetBoxBR = vec2(0.0f);
	vec2 navPoint = vec2(0.0f);

	CockpitConsoleMode consoleMode = CockpitConsoleMode::target;
};
