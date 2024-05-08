#pragma once

#include "WCConversation.h"
#include "WCScene.h"

class WCArchive;

class WCGameData
{
public:
	WCGameData(WCArchive* archive);

	std::map<std::string, std::string> files;

	int landFee = 0;
	std::map<std::string, std::string> apprCock;
	std::vector<std::string> commText;
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
