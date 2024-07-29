#pragma once

#include "FileFormat/WCArchive.h"
#include "FileFormat/WCGameData.h"
#include "FileFormat/WCSector.h"
#include "Audio/AudioSource.h"

class GameScreen;
class AudioPlayer;
class WCVOCSound;

class GameApp
{
public:
	GameApp();
	~GameApp();

	int main(std::vector<std::string> args);

	void ShowScreen(std::unique_ptr<GameScreen> screen);
	void PushScreen(std::unique_ptr<GameScreen> screen);
	void PopScreen();

	void PlayMusic(std::string filename, int song);
	void PlaySound(WCVOCSound* sound);

	std::unique_ptr<WCArchive> archive;
	std::unique_ptr<WCGameData> gamedata;
	std::unique_ptr<WCSectorData> sectordata;
	std::vector<std::unique_ptr<GameScreen>> screenStack;
	std::vector<std::unique_ptr<GameScreen>> screenDeleteList;

	std::unique_ptr<AudioPlayer> musicPlayer;
	std::unique_ptr<AudioPlayer> soundPlayer;
	ZMusic_MusicStream musicStream;
};
