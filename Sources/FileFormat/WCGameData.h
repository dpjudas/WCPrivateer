#pragma once

#include "WCConversation.h"
#include "WCScene.h"
#include "WCImage.h"

class WCArchive;

class WCCommodity
{
public:
	std::string label;
	int info0 = -1;
	int info1 = -1;
	std::vector<int16_t> cost;
	std::vector<int16_t> availability;
};

class WCCommodityList
{
public:
	std::vector<WCCommodity> commodities;
	std::vector<uint8_t> from, sell, dest;
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
	std::unique_ptr<WCImage> convFont;
	std::vector<std::string> pcString;
	std::vector<std::string> shipMText;
	std::vector<std::string> softText;
	std::map<std::string, std::string> typeNames;

	std::unique_ptr<WCConvBackgroundList> convBackgroundList;
	std::unique_ptr<WCConvFaceList> convFaceList;
	std::unique_ptr<WCConvEyesList> convEyesList;

	std::unique_ptr<WCSceneList> sceneList;

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
	std::vector<std::string> LoadStringList(const std::string& filename);

	WCArchive* archive = nullptr;
};
