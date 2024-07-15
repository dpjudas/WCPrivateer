#pragma once

#include "WCImage.h"

class WCArchive;
class WCPalette;

class WCSpaceSprite
{
public:
	WCSpaceSprite(std::string name, WCArchive* archive);
	~WCSpaceSprite();

	std::unique_ptr<WCImage> shape;
};
