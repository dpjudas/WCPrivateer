#pragma once

#include "WCImage.h"

class WCArchive;
class WCPalette;

class WCSpaceSprite
{
public:
	WCSpaceSprite(std::string name, WCArchive* archive);
	~WCSpaceSprite();

	std::vector<uint8_t> info;
	std::unique_ptr<WCImage> shape;
	std::unique_ptr<WCImage> target;
	std::vector<uint8_t> skel;
};

class WCSpaceShip
{
public:
	WCSpaceShip(std::string name, WCArchive* archive);
	~WCSpaceShip();

	std::vector<std::unique_ptr<WCImage>> shapes;
	std::unique_ptr<WCImage> target;
	std::unique_ptr<WCImage> weapon;
	std::unique_ptr<WCImage> burn;
	std::vector<uint8_t> skel;
	std::vector<uint8_t> scor;
};
