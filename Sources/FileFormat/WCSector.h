#pragma once

class WCArchive;
class FileEntryReader;

struct WCSectorTableEntry
{
	uint8_t data[68] = {};
};

enum class WCSectorBaseType
{
	unknown,
	pleasure,
	refinery,
	mining,
	agricultural,
	pirate,
	special
};

struct WCSectorBase
{
	std::string name;
	int index = 0;
	WCSectorBaseType type = {};
};

struct WCSystem
{
	std::string name;
	int sectorIndex = 0;
	int x = 0;
	int y = 0;
	std::vector<int> baseIndexes; // Index into WCSectorData.bases for the base in the system
};

struct WCQuadrant
{
	std::string name;
	int x = 0;
	int y = 0;
	std::vector<WCSystem> systems;
};

struct WCSectorTeam
{
	std::vector<uint8_t> data;
};

struct WCSectorSun
{
	std::string name;
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
};

struct WCSectorJumpItem
{
	std::string name;
	std::string type;
	int encounterIndex = 0;
	uint8_t unknown2[2] = {};
	int sphereIndex = 0;
	uint8_t unknown25[25] = {};
};

struct WCSectorBaseItem
{
	std::string name;
	std::string type;
	int a = 0;
	int b = 0;
	uint8_t unknown5[5] = {};
	int x = 0;
	int y = 0;
	int z = 0;
	uint8_t unknown10[10] = {};
	int baseIndex = 0;
};

struct WCSectorEncounterItem
{
	std::string name;
	std::string type;
	int chance = 0; // In a roll 0-100, the bracket closest to the number will spawn for a given scene
	int encounterIndex = 0;
	uint8_t unknown2[2] = {};
	int sphereIndex = 0;
	uint8_t unknown24[24] = {};
};

struct WCSectorSphere
{
	int sphereIndex = 0;
	int sectorIndex = 0;
	int x = 0;
	int y = 0;
	int z = 0;
	int argtype = 0; // 7 is navigation points, 46, 66, 86 also seen
	int arg0 = 0; // for nav points, 0 is nothing, 1 is a base, 2 is a jump point
	int arg1 = 0; // for nav points, base index or sector index
	int arg2 = 0; // for nav points, the nav point number
};

struct WCSectorScene
{
	int a = 0;
	int index = 0;
	int sectorIndex = 0;
	int b = 0;
	int c = 0;
	int d = 0;
	std::vector<int16_t> encounters;
};

struct WCSector
{
	std::string label;
	int name = 0;
	int race[2] = {};
	std::vector<WCSectorSphere> spheres;

	std::vector<std::string> cast;
	int flag = -1;
	std::vector<int16_t> prog;
	std::vector<uint8_t> cnst;
	std::vector<WCSectorEncounterItem> encounters;
	std::vector<WCSectorJumpItem> jumps;
	std::vector<WCSectorBaseItem> bases;
	std::vector<uint8_t> ojmp;
	std::vector<WCSectorScene> scenes;
	std::vector<std::string> objtypes;
	std::vector<std::string> appr;

	std::string starspak;
	std::vector<int16_t> stars;
	std::vector<WCSectorSun> suns;
};

class WCSectorData
{
public:
	WCSectorData(WCArchive* archive);

	uint8_t basedata[6] = {};
	std::vector<WCSectorBase> bases;
	std::vector<WCQuadrant> quadrants;
	std::vector<WCSectorTeam> teams;
	std::vector<WCSector> sectors;
	std::vector<WCSectorTableEntry> table;

private:
	void LoadBases(WCArchive* archive);
	void LoadQuadrant(WCArchive* archive);
	void LoadSectors(WCArchive* archive);
	void LoadTable(WCArchive* archive);
	void LoadTeams(WCArchive* archive);
};
