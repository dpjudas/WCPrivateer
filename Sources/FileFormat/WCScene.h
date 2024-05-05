#pragma once

#include "WCImage.h"

class WCArchive;
class WCPalette;

class WCRegionPoint
{
public:
	int x = 0;
	int y = 0;
};

enum class WCTarget : int
{
	None = -1,
	ActivateComputer = 1,
	SelectDisc1 = 2,
	SelectDisc2 = 3,
	SelectDisc3 = 4,
	AcceptMission = 5,
	Library = 26,
	ResearchComputers = 27,
	AdmiralsOffice = 32,
	MerchantsGuild = 61,
	CommodityExchange = 63,
	RepairAndUpgrade = 69,
	PurchaseCenturion = 99,
	PurchaseGalaxy = 192,
	PurchaseOrion = 193,
	AnalyzeArtifact = 194,
	MissionComputer = 200,
	ShipDealer = 201,
	MainConcourse = 202,
	MercenariesGuild = 203,
	Bar = 206,
	LandingPad = 207,
	Computer = 225
};

enum class WCSpriteTarget : int
{
	None = -1,
	PreviousMission = 2,
	NextMission = 3,
	AcceptMission = 4,
	TalkToSandoval = 0,
	TalkToTayla = 1,
	TalkToRomanLynch = 2,
	TalkToMiggs = 3,
	TalkToLynnMurphy = 4,
	TalkToDrMonkhouse = 5,
	TalkToTarynCross = 6,
	TalkToSandraGoodin = 7,
	TalkToRomanLynch2 = 13,
	TalkToAdmiralTerrell = 198,
	TrainToOxfordUniversity = 202,
	Hangar = 207,
	TalkToBartender = 216,
	TalkToGuild = 223, // receptionist (mercenary) or guild master (merchant)
	RemoveDerelictWeapon = 233
};

class WCRegion
{
public:
	WCTarget target = WCTarget::None;
	std::string label; // Ship Dealer, Misson Computer
	std::vector<WCRegionPoint> coords;
};

class WCSceneShape
{
public:
	int unknown16 = 0;
	int optpakIndex = 0;
	int offsetX = 0;
	int offsetY = 0;
	std::vector<uint8_t> unknown;
};

class WCSceneSprite
{
public:
	WCSpriteTarget target = WCSpriteTarget::None;
	int optpakIndex = 0;
	std::vector<uint8_t> shape;
	std::string label; // Talk to Bartender, Talk to Dr. Monkhouse
	std::vector<uint16_t> info;
	std::vector<uint16_t> sequence;
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	bool clck = false;
	std::vector<WCRegionPoint> region;
};

class WCSceneBackground
{
public:
	int palette = 0;
	std::vector<WCSceneShape> shapes;
};

class WCSceneForeground
{
public:
	int palette = 0;
	std::vector<WCSceneSprite> sprites;
};

class WCSceneShip
{
public:
	std::vector<uint8_t> info;
	std::unique_ptr<WCImage> shape;
};

class WCSceneLook
{
public:
	std::vector<uint8_t> info;
};

class WCScene
{
public:
	std::vector<WCRegion> regions;
	WCSceneBackground background;
	WCSceneForeground foreground;
	WCSceneLook look;
	WCSceneShip ship;
};

class WCSceneList
{
public:
	WCSceneList(WCArchive* archive);

	std::vector<WCScene> scenes;

private:
	WCRegion ReadRegion(FileEntryReader& reader);
	WCSceneBackground ReadBackground(FileEntryReader& reader);
	WCSceneForeground ReadForeground(FileEntryReader& reader);
	WCSceneLook ReadLook(FileEntryReader& reader);
	WCSceneShip ReadShip(FileEntryReader& reader, WCPalette* palette);
};
