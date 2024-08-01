#pragma once

#include "WCImage.h"

class WCArchive;
class FileEntryReader;

class WCNavMapType
{
public:
	WCNavMapType(WCArchive* archive);

	std::unique_ptr<WCImage> background;
	std::unique_ptr<WCImage> grid;
	std::unique_ptr<WCImage> mouse;
	struct
	{
		std::unique_ptr<WCImage> quad;
		std::unique_ptr<WCImage> navi;
		std::unique_ptr<WCImage> next;
	} button;

	struct
	{
		int16_t main[4] = {};
		int16_t info[4] = {};
		int16_t button[12] = {};
	} wndw;

	uint8_t vars[16] = {};
	uint8_t color[8] = {};
	std::vector<std::string> obje, basn, bass;
};
