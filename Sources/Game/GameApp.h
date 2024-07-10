#pragma once

#include "FileFormat/WCArchive.h"
#include "FileFormat/WCGameData.h"

class GameScreen;

class GameApp
{
public:
	GameApp();
	~GameApp();

	int main(std::vector<std::string> args);

	void ShowScreen(std::unique_ptr<GameScreen> screen);
	void PushScreen(std::unique_ptr<GameScreen> screen);
	void PopScreen();

	std::unique_ptr<WCArchive> archive;
	std::unique_ptr<WCGameData> gamedata;
	std::vector<std::unique_ptr<GameScreen>> screenStack;
	std::vector<std::unique_ptr<GameScreen>> screenDeleteList;
};
