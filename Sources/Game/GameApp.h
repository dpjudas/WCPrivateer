#pragma once

#include "FileFormat/WCArchive.h"

class GameApp
{
public:
	int main(std::vector<std::string> args);

	std::unique_ptr<WCArchive> archive;
};
