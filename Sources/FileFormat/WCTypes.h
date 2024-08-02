#pragma once

#include "WCImage.h"

class WCArchive;
class FileEntryReader;

class WCAstroidType
{
public:
	WCAstroidType(WCArchive* archive);
	int16_t info[11] = {};
};

class WCBaseType
{
public:
	WCBaseType(WCArchive* archive);
};

class WCBeamType
{
public:
	WCBeamType(WCArchive* archive);

	int info0 = 0;
	int info1 = 0;
	int info2 = 0;
	std::vector<uint8_t> beamhaze;

	int sphereinfo0 = 0;
	int sphereinfo1 = 0;
	std::vector<uint8_t> spherehaze;
};

class WCJumpType
{
public:
	WCJumpType(WCArchive* archive);

	int info = 0;
	int hazeinfo0 = 0;
	int hazeinfo1 = 0;
	std::vector<uint8_t> haze;
};

class WCFighterType
{
public:
	WCFighterType(const std::string& filename, WCArchive* archive);

	std::string control;
	std::vector<uint8_t> guns;
	std::vector<uint8_t> launch;
	std::vector<uint8_t> missile;
	std::string energyName;
	std::vector<uint8_t> energyInfo;
	std::vector<uint8_t> energyDamage;
	std::string shieldName;
	std::vector<uint8_t> shieldInfo;
	std::vector<uint8_t> shieldArmor;
	std::vector<uint8_t> shieldDamage;
	std::string cargoName;
	std::vector<uint8_t> cargoInfo;
	std::vector<uint8_t> cargoData;
	std::vector<uint8_t> cargoDamage;
	std::string commName;
	std::vector<uint8_t> commInfo;
	std::vector<uint8_t> turret;
};

class WCBrodType : public WCFighterType
{
public:
	WCBrodType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\BRODTYPE.IFF", archive) { }
};

class WCCenturionType : public WCFighterType
{
public:
	WCCenturionType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\CENTTYPE.IFF", archive) { }
};

class WCDemonType : public WCFighterType
{
public:
	WCDemonType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\DEMNTYPE.IFF", archive) { }
};

class WCDralType : public WCFighterType
{
public:
	WCDralType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\DRALTYPE.IFF", archive) { }
};

class WCDrayType : public WCFighterType
{
public:
	WCDrayType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\DRAYTYPE.IFF", archive) { }
};

class WCDronType : public WCFighterType
{
public:
	WCDronType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\DRONTYPE.IFF", archive) { }
};

class WCGladiatorType : public WCFighterType
{
public:
	WCGladiatorType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\GLADTYPE.IFF", archive) { }
};

class WCGalaxyType : public WCFighterType
{
public:
	WCGalaxyType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\GLXYTYPE.IFF", archive) { }
};

class WCGothType : public WCFighterType
{
public:
	WCGothType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\GOTHTYPE.IFF", archive) { }
};

class WCKamkType : public WCFighterType
{
public:
	WCKamkType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\KAMKTYPE.IFF", archive) { }
};

class WCOrionType : public WCFighterType
{
public:
	WCOrionType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\ORIOTYPE.IFF", archive) { }
};

class WCParaMilitaryType : public WCFighterType
{
public:
	WCParaMilitaryType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\PARATYPE.IFF", archive) { }
};

class WCPirateType : public WCFighterType
{
public:
	WCPirateType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\PIRATYPE.IFF", archive) { }
};

class WCSSCTType : public WCFighterType
{
public:
	WCSSCTType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\SSCTTYPE.IFF", archive) { }
};

class WCStilType : public WCFighterType
{
public:
	WCStilType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\STILTYPE.IFF", archive) { }
};

class WCTalnType : public WCFighterType
{
public:
	WCTalnType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\TALNTYPE.IFF", archive) { }
};

class WCTalnWimpType : public WCFighterType
{
public:
	WCTalnWimpType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\TALNWIMP.IFF", archive) { }
};

class WCTarsType : public WCFighterType
{
public:
	WCTarsType(WCArchive* archive) : WCFighterType("DATA\\TYPES\\TARSTYPE.IFF", archive) { }
};

class WCProjectileType
{
public:
	WCProjectileType(const std::string& filename, WCArchive* archive);

	std::vector<uint8_t> info;
};

class WCIonProjType : public WCProjectileType
{
public:
	WCIonProjType(WCArchive* archive) : WCProjectileType("DATA\\TYPES\\IONCTYPE.IFF", archive) { }
};

class WCMassProjType : public WCProjectileType
{
public:
	WCMassProjType(WCArchive* archive) : WCProjectileType("DATA\\TYPES\\MASSTYPE.IFF", archive) { }
};

class WCMesnType : public WCProjectileType
{
public:
	WCMesnType(WCArchive* archive) : WCProjectileType("DATA\\TYPES\\MESNTYPE.IFF", archive) { }
};

class WCNeutronProjType : public WCProjectileType
{
public:
	WCNeutronProjType(WCArchive* archive) : WCProjectileType("DATA\\TYPES\\NTRNTYPE.IFF", archive) { }
};

class WCParticleProjType : public WCProjectileType
{
public:
	WCParticleProjType(WCArchive* archive) : WCProjectileType("DATA\\TYPES\\PARTTYPE.IFF", archive) { }
};

class WCPlasmaProjType : public WCProjectileType
{
public:
	WCPlasmaProjType(WCArchive* archive) : WCProjectileType("DATA\\TYPES\\PLSMTYPE.IFF", archive) { }
};

class WCTachyonProjType : public WCProjectileType
{
public:
	WCTachyonProjType(WCArchive* archive) : WCProjectileType("DATA\\TYPES\\TCHNTYPE.IFF", archive) { }
};

class WCTorpedoType : public WCProjectileType
{
public:
	WCTorpedoType(WCArchive* archive) : WCProjectileType("DATA\\TYPES\\TORPTYPE.IFF", archive) { }
};

class WCNavMapType
{
public:
	WCNavMapType(WCArchive* archive);

	std::unique_ptr<WCImage> background;
	std::unique_ptr<WCImage> grid;
	std::unique_ptr<WCImage> mouse;
	struct
	{
		std::unique_ptr<WCImage> quad;
		std::unique_ptr<WCImage> navi;
		std::unique_ptr<WCImage> next;
	} button;

	struct
	{
		int16_t main[4] = {};
		int16_t info[4] = {};
		int16_t button[12] = {};
	} wndw;

	uint8_t vars[16] = {};
	uint8_t color[8] = {};
	std::vector<std::string> obje, basn, bass;
};

class WCTargetingTarget
{
public:
	int name = 0;
	std::vector<int16_t> radarInfo;
	std::unique_ptr<WCImage> radarShape;
	int16_t damage0 = 0;
	int16_t damage1 = 0;
	std::vector<uint8_t> info;
};

class WCTargetingType
{
public:
	WCTargetingType(WCArchive* archive);

	std::vector<WCTargetingTarget> targets;
};
