#pragma once

#include "WCConversation.h"
#include "WCScene.h"
#include "WCImage.h"

class WCArchive;

enum class WCCommodityLocation
{
	agricultural,
	mining,
	newconstantinople,
	newdetroit,
	oxford,
	perry,
	pirate,
	pleasure,
	refinery
};

class WCCommoditySet
{
public:
	WCCommodityLocation location = {};
	int unused = 0;
	int modifier = -1; // -1 = not available at this location. Otherwise range is [base, base - modifier + 1]
};

class WCCommodity
{
public:
	std::string label;
	int info0 = -1;
	int info1 = -1;
	int baseCost = 0; // price of item
	int baseAvailability = 0; // number of items
	std::vector<WCCommoditySet> cost;
	std::vector<WCCommoditySet> availability;
};

class WCCommodityList
{
public:
	std::vector<WCCommodity> commodities;
	std::vector<uint8_t> from, sell, dest;
};

enum class WCCommodityText
{
	Price,
	Quantity,
	Cost,
	Credits,
	Thanks,
	NoSell,
	NoBuy,
	NoCredit,
	NoRoom
};

enum class WCSoftwareText
{
	Price,
	Mode,
	Thanks,
	NoCredit,
	Buy,
	Sell,
	Repair,
	NoRoom,
	NotValid,
	NoSell,
	NoRepair,
	ItemRepaired,
	Credits
};

enum class WCShipMText
{
	Price,
	ShipsQuantity,
	Quantity,
	Cost,
	Credits,
	Thanks,
	NoSell,
	AbortTransaction,
	OrdnanceRemoved,
	TurretDismounted,
	NoCredit,
	NoRoom,
	OrdnanceLoaded,
	TurretMounted,
	NoRepair,
	TooMuchCargo
};

class WCShipStuffItem
{
public:
	std::vector<uint8_t> info;
	std::string label;
	uint32_t buyprice = 0;
	uint32_t sellprice = 0;
	std::unique_ptr<WCImage> shape;
};

class WCLimitNamed
{
public:
	std::string name;
	std::vector<uint8_t> unknown;
};

class WCLimitEngine
{
public:
	uint8_t info = 0;
	int16_t genr[3] = {};
};

class WCLimits
{
public:
	std::vector<WCLimitNamed> named;
	std::vector<WCLimitEngine> engines;
};

class WCGun
{
public:
	std::string shortname;
	std::string longname;

	uint32_t abuse = 0;
	uint32_t velocity = 0;
	uint32_t lifetime = 0; // fixed point, divide by 256 to get seconds
	uint32_t refirerate = 0; // fixed point, divide by 256 to get seconds
	uint16_t energy = 0; // divide by 10 to get gigajoule value
	int16_t damage = 0; // armor penetration
	uint8_t unknown0 = 0;
	uint32_t unknown1 = 0;

	// "The Range can be calculated by multiplying the Lifetime and Velocity.
	// Note that the unit kps can not mean kilometers per second. I think it
	// means klicks per second and klick means meter. So kps is the same as
	// mps or m/s (meters per second)."
};

class WCLaunchWeapon
{
public:
	uint16_t velocity = 0;
	uint16_t lifetime = 0; // in seconds
	uint16_t unknown = 0;
	uint8_t damage = 0;
	std::vector<uint8_t> unknown2;
};

class WCMissileWeapon
{
public:
	uint8_t index = 0;
	std::string shortname;
	std::string longname;
	uint16_t velocity = 0;
	uint16_t lifetime = 0; // in seconds
	uint16_t unknown0 = 0;
	uint8_t damage = 0;
	std::vector<uint8_t> unknown1;
};

class WCMissionText
{
public:
	std::vector<std::string> corporations;
	std::vector<std::string> enemies;
	std::map<std::string, std::string> summary;
	std::map<std::string, std::string> copy;
};

class WCGameFlowSprite
{
public:
	int info = 0;
	std::vector<uint8_t> effect;
	std::vector<uint8_t> request;
};

class WCGameFlowScene
{
public:
	int sceneIndex = 0;
	std::vector<WCGameFlowSprite> sprites;
};

class WCGameFlowMission
{
public:
	int info = 0;
	int tune = 0;
	int startSceneIndex = 0;
	std::vector<WCGameFlowScene> scenes;
};

class WCGameFlow
{
public:
	std::vector<WCGameFlowMission> missions;
	std::vector<std::string> conversations;
};

class WCShipArtGunType
{
public:
	int info = 0;
	std::vector<uint8_t> desc;
	std::unique_ptr<WCImage> shape;
};

class WCShipArt
{
public:
	std::unique_ptr<WCImage> shape;
	std::vector<WCShipArtGunType> guns;
	std::vector<WCShipArtGunType> misc;
};

class WCGameData
{
public:
	WCGameData(WCArchive* archive);

	std::map<std::string, std::string> files;

	int landFee = 0;
	std::map<std::string, std::string> apprCock;
	std::vector<std::string> commText;
	std::vector<std::unique_ptr<WCImage>> commStuff;
	std::unique_ptr<WCCommodityList> commodityList;
	std::map<std::string, std::string> merchantText;
	std::map<std::string, std::string> mercenaryText;
	std::map<std::string, std::string> missionComputerText;
	WCMissionText missionText;
	std::unique_ptr<WCImage> convFont;
	std::vector<std::string> pcString;
	std::vector<std::string> shipMText;
	std::vector<std::string> softText;
	std::map<std::string, std::string> typeNames;
	std::vector<WCShipStuffItem> shipStuff;
	std::vector<WCShipStuffItem> softwareStuff;
	std::map<std::string, WCShipArt> shipArt;

	std::unique_ptr<WCConvBackgroundList> convBackgroundList;
	std::unique_ptr<WCConvFaceList> convFaceList;
	std::unique_ptr<WCConvEyesList> convEyesList;

	WCGameFlow gameFlow;
	std::unique_ptr<WCSceneList> sceneList;

	WCLimits limits;

	std::vector<WCGun> guns;
	std::vector<WCLaunchWeapon> launchWeapons;
	std::vector<WCMissileWeapon> missileWeapons;

private:
	void LoadFiles();
	void LoadApprCock();
	void LoadCommStuf();
	void LoadCommTxt();
	void LoadComodtyp();
	void LoadCompText();
	void LoadCU();
	void LoadEyes();
	void LoadFaces();
	void LoadFonts();
	void LoadGameFlow();
	void LoadLandFee();
	void LoadLimits();
	void LoadMisnText();
	void LoadOptions();
	void LoadPCString();
	void LoadShipArt();
	void LoadShipMTxt();
	void LoadShipStuf();
	void LoadSoftTxt();
	void LoadTypeNames();
	void LoadGuns();
	void LoadWeapons();
	std::vector<std::string> LoadStringList(const std::string& filename);

	WCArchive* archive = nullptr;
};
