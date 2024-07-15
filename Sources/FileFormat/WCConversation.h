#pragma once

#include "WCPalette.h"

class WCArchive;

class WCConvBackground
{
public:
	std::string shape;
	int pakindex = 0;
	std::unique_ptr<WCPalette> palette;
};

class WCConvBackgroundList
{
public:
	WCConvBackgroundList(WCArchive* archive);

	std::vector<WCConvBackground> backgrounds;
	std::vector<std::pair<int, std::string>> backdata;
};

class WCConvFace
{
public:
	std::string name;

	int eyesX = 0;
	int eyesY = 0;
	int mouthX = 0;
	int mouthY = 0;
	int uniformX = 0;
	int uniformY = 0;
	int unknown0X = 0;
	int unknown0Y = 0;
	int unknown1X = 0;
	int unknown1Y = 0;
	int unknown2X = 0;
	int unknown2Y = 0;
	int handX = 0;
	int handY = 0;

	int head = -1;
	int eyes = -1;
	int mouth = -1;
	int uniform = -1;
	int hand = -1;
};

class WCConvFaceList
{
public:
	WCConvFaceList(WCArchive* archive);

	std::unique_ptr<WCPalette> palette;
	std::vector<WCConvFace> faces;
};

class WCConvEyes
{
public:
	std::string name;
	uint16_t unknown;
	std::vector<uint8_t> eyes;
};

class WCConvEyesDef
{
public:
	std::vector<uint8_t> def;
};

class WCConvEyesList
{
public:
	WCConvEyesList(WCArchive* archive);

	std::vector<WCConvEyes> eyesList;
	std::vector<WCConvEyesDef> defs;
	std::vector<uint8_t> medfdata;
};

class WCConversationStep
{
public:
	std::string actorname;
	std::string normal; // is this ever any other string than "normal"?
	std::string location;
	int16_t unknown0; // 0x9f (159)
	int16_t unknown1; // 9, 13, 14
	std::string text;
	std::vector<uint8_t> unknown2;
};

class WCConversation
{
public:
	WCConversation(const std::string& name, WCArchive* archive);

	std::vector<WCConversationStep> steps;
};
