#pragma once

#include "FileFormat/WCArchive.h"
#include "FileFormat/WCGameData.h"

class GameApp
{
public:
	int main(std::vector<std::string> args);

	std::unique_ptr<WCArchive> archive;
	std::unique_ptr<WCGameData> gamedata;
};
