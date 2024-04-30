#pragma once

#include "WCImage.h"

class WCArchive;
class WCPalette;

class WCRegion
{
public:
	int target = 0;
	std::string label; // Ship Dealer, Misson Computer
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
};

class WCSceneShape
{
public:
	int type = 0;
	int optpakIndex = 0;
	std::vector<uint8_t> unknown;
};

class WCSceneSprite
{
public:
	std::vector<uint8_t> shape;
	std::string label; // Talk to Bartender, Talk to Dr. Monkhouse
	std::vector<uint8_t> info;
	std::vector<uint8_t> sequence;
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	bool clck = false;
	struct
	{
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;
	} region;
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
