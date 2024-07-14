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

class WCCockpitEnergyView
{
public:
	int x0 = 0;
	int y0 = 0;
	int x1 = 0;
	int y1 = 0;
	std::vector<uint8_t> data;
	std::unique_ptr<WCImage> shape;
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

	struct
	{
		int x0 = 0;
		int y0 = 0;
		int x1 = 0;
		int y1 = 0;
		uint8_t unknown[3] = {};
	} cmfd;
	std::string software;

	struct
	{
		int x = 0;
		int y = 0;
		std::string text;
	} setSpeed, actualSpeed;

	struct
	{
		std::vector<uint8_t> hinf;
		std::vector<uint8_t> trgtinfo;
		std::vector<uint8_t> damg;
	} chud;

	std::vector<uint8_t> guns;

	std::unique_ptr<WCImage> itts;
	std::unique_ptr<WCImage> crosshair;
	std::unique_ptr<WCImage> navigationCrosshair;
	std::unique_ptr<WCImage> shield;
	std::unique_ptr<WCImage> fuel;
	std::unique_ptr<WCImage> autopilot;
	std::vector<WCCockpitEnergyView> energy;

	struct
	{
		int x0 = 0;
		int y0 = 0;
		int x1 = 0;
		int y1 = 0;
	} radarPos, shieldPos, fuelPos, autoPos;

	struct
	{
		struct
		{
			std::vector<uint8_t> data;
		} shld, fuel;
	} dial;

	std::vector<uint8_t> damg;

	struct
	{
		std::vector<uint8_t> explosion;
		std::vector<uint8_t> sprite;
	} cdmg;
};

class WCSpaceSprite
{
public:
	WCSpaceSprite(std::string name, WCArchive* archive);
	~WCSpaceSprite();

	std::unique_ptr<WCImage> shape;
};
