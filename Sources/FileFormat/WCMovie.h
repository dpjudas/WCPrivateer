#pragma once

class WCArchive;
class FileEntryReader;

class WCMovieFile
{
public:
	int slot = 0;
	std::string filename;
};

class WCMovieBlock12
{
public:
	uint16_t data[12];
};

class WCMovieScene
{
public:
	std::vector<int16_t> fild;
	std::vector<uint8_t> spri;
	std::vector<WCMovieBlock12> bfor;
};

class WCMovie
{
public:
	WCMovie(const std::string& filename, WCArchive* archive);

	int16_t clrc = -1;
	int16_t speed = -1;
	std::vector<WCMovieFile> files;
	std::vector<int16_t> fild;
	std::vector<uint8_t> spri;

	struct
	{
		std::vector<int16_t> fild;
		std::vector<uint8_t> spri;
		std::vector<WCMovieBlock12> bfor; // seems to be int16[12] blocks?
		std::vector<WCMovieBlock12> aftr;
		std::vector<WCMovieScene> scenes;
	} acts;
};

class WCShipToName // ship2chr.iff
{
public:
	WCShipToName(WCArchive* archive);

	std::map<int, std::string> names; // 0 -> CLUNKER, 1 -> TUG, 2 -> FIGHTER, 3 -> MERCHANT
};

class WCMidgames // midgames.iff and gfmidgam.iff
{
public:
	WCMidgames(const std::string& filename, WCArchive* archive);

	std::vector<std::string> filenames; // JUMP.IFF, DISCOVER.IFF, VICTLIST.PAK
};

class WCDeathAppearance // deathappr.iff
{
public:
	WCDeathAppearance(WCArchive* archive);

	std::map<std::string, std::string> deathNames; // FIGHTER -> FITEDETH, MERCHANT -> MRCHDETH, TUG -> TUGDETH, CLUNKER -> CLNKDETH
};
