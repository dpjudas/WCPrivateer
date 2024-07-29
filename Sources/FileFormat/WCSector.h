#pragma once

class WCArchive;
class FileEntryReader;

struct WCSectorTableEntry
{
	uint8_t data[68] = {};
};

struct WCSectorBase
{
	std::string name;
	int index = 0;
	int type = 0;
};

struct WCSystem
{
	std::string name;
	int a = 0;
	int b = 0;
	int c = 0;
	std::vector<uint8_t> base;
};

struct WCQuadrant
{
	std::string name;
	int a = 0;
	int b = 0;
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

struct WCSector
{
	std::string label;
	int name = 0;
	int race[2] = {};
	std::vector<uint8_t> sphr;

	std::vector<std::string> cast;
	uint32_t flag = 0;
	std::vector<uint8_t> prog;
	std::vector<uint8_t> cnst;
	std::vector<uint8_t> wand;
	std::vector<uint8_t> jump;
	std::vector<uint8_t> base;
	std::vector<uint8_t> ojmp;
	std::vector<std::vector<uint8_t>> scenes;
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
