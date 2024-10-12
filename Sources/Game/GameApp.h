#pragma once

#include "FileFormat/WCArchive.h"
#include "FileFormat/WCGameData.h"
#include "FileFormat/WCSector.h"
#include "Audio/AudioSource.h"
#include "Math/Vec.h"
#include <cmath>

class GameScreen;
class GameObject;
class PlayerPawn;
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

	float Random(float minval, float maxval) { return minval + (maxval - minval) * rand() / RAND_MAX; }
	int Random(int minval, int maxval) { return (int)std::round(Random((float)minval, (float)maxval)); }

	struct
	{
		vec3 spawnPoint = vec3(60000.0f, 2500.0f, -40000.0f);
		std::vector<std::unique_ptr<GameObject>> gameObjects;
		PlayerPawn* player = nullptr;
		int sectorIndex = 58;
		int navpoint = 2;
		bool autopilotAllowed = false;
		bool paused = false;
	} playsim;

	int64_t GetGameTime();
	int64_t StartTimeNS = 0;

	int64_t FrameStartTime = 0;
	float TimeElapsed = 0.0f;

	std::unique_ptr<WCArchive> archive;
	std::unique_ptr<WCGameData> gamedata;
	std::unique_ptr<WCSectorData> sectordata;
	std::vector<std::unique_ptr<GameScreen>> screenStack;
	std::vector<std::unique_ptr<GameScreen>> screenDeleteList;

	std::unique_ptr<AudioPlayer> musicPlayer;
	std::unique_ptr<AudioPlayer> soundPlayer;
	ZMusic_MusicStream musicStream;
};
