#pragma once

#include "WCImage.h"

class WCArchive;
class WCPalette;

class WCCockpitViewpoint
{
public:
	std::unique_ptr<WCImage> shape;
	std::vector<uint8_t> tplt; // what is this?
};

class WCCockpit
{
public:
	WCCockpit(std::string name, WCArchive* archive);
	~WCCockpit();

	WCCockpitViewpoint front;
	WCCockpitViewpoint back;
	WCCockpitViewpoint left;
	WCCockpitViewpoint right;
	WCCockpitViewpoint turret1;
	WCCockpitViewpoint turret2;

	struct
	{
		int unknown0 = 0;
		int unknown1 = 0;
	} trrt;
	int offset[2] = {};
	std::string font;
};

class WCSpaceSprite
{
public:
	WCSpaceSprite(std::string name, WCArchive* archive);
	~WCSpaceSprite();

	std::unique_ptr<WCImage> shape;
};
