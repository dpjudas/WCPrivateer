
#include "IntroScreen.h"
#include "FileFormat/WCMovie.h"
#include "FileFormat/WCPak.h"
#include "FileFormat/WCSound.h"
#include "Game/GameApp.h"
#include "Game/Screens/MainMenuScreen.h"
#include <chrono>

IntroScreen::IntroScreen(GameApp* app) : GameScreen(app)
{
	SetScene(0);
}

IntroScreen::~IntroScreen()
{
}

void IntroScreen::SetScene(int index)
{
	currentScene = index;
	currentSound = -1;

	if (currentScene == 0) // Planet panning scene
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1A.IFF", app->archive.get());

		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		WCPak pak(textpak, app->archive.get());
		for (size_t i = 0; i < 4; i++)
		{
			std::vector<uint8_t> buf = pak.files[i];
			buf.push_back(0);
			scenetext.push_back((const char*)buf.data());
		}

		palette = LoadPakPalette(imagepak, 0);
		scene.clear();
		scene.push_back(LoadPakImage(imagepak, 1, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 2, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 3, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 4, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 5, palette.get()));

		font = LoadShpImage(movie->files[2].filename, palette.get()); // slot 2
		PlayMusic(movie->files[3].filename + ".GEN", 0); // slot 4
	}
	else if (currentScene == 1) // cockpit + ship chase scene
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1B.IFF", app->archive.get());

		std::string imagepak = movie->files[0].filename; // slot 0
		palette = LoadPakPalette(imagepak, 6);
		scene.clear();
		scene.push_back(LoadPakImage(imagepak, 7, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 8, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 9, palette.get()));
		palette = LoadPakPalette(imagepak, 10);
		scene.push_back(LoadPakImage(imagepak, 11, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 12, palette.get()));
	}
	else if (currentScene == 2) // pirate conversation
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1C1.IFF", app->archive.get());

		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		WCPak pak(textpak, app->archive.get());
		scenetext.clear();
		for (size_t i = 4; i < 7; i++)
		{
			std::vector<uint8_t> buf = pak.files[i];
			buf.push_back(0);
			scenetext.push_back((const char*)buf.data());
		}

		palette = LoadPakPalette(imagepak, 10);
		font = LoadShpImage(movie->files[2].filename, palette.get()); // slot 6

		sounds.clear();
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 61
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[4].filename, app->archive.get())); // slot 62
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[5].filename, app->archive.get())); // slot 63

		scene.clear();
	}
	else if (currentScene == 3) // protagonist replying back conversation
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1C2.IFF", app->archive.get());

		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		WCPak pak(textpak, app->archive.get());
		scenetext.clear();
		for (size_t i = 7; i < 9; i++)
		{
			std::vector<uint8_t> buf = pak.files[i];
			buf.push_back(0);
			scenetext.push_back((const char*)buf.data());
		}

		palette = LoadPakPalette(imagepak, 10);
		font = LoadShpImage(movie->files[2].filename, palette.get()); // slot 6

		sounds.clear();
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 94
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[4].filename, app->archive.get())); // slot 95

		scene.clear();
	}
	else if (currentScene == 4) // pirate conv 2
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1C3.IFF", app->archive.get());

		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		WCPak pak(textpak, app->archive.get());
		scenetext.clear();
		for (size_t i = 9; i < 11; i++)
		{
			std::vector<uint8_t> buf = pak.files[i];
			buf.push_back(0);
			scenetext.push_back((const char*)buf.data());
		}

		palette = LoadPakPalette(imagepak, 10);
		font = LoadShpImage(movie->files[2].filename, palette.get()); // slot 6

		sounds.clear();
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 116
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[4].filename, app->archive.get())); // slot 117

		scene.clear();
	}
	else if (currentScene == 5) // protagonist conv 2
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1C4.IFF", app->archive.get());

		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		WCPak pak(textpak, app->archive.get());
		scenetext.clear();
		for (size_t i = 11; i < 13; i++)
		{
			std::vector<uint8_t> buf = pak.files[i];
			buf.push_back(0);
			scenetext.push_back((const char*)buf.data());
		}

		palette = LoadPakPalette(imagepak, 10);
		font = LoadShpImage(movie->files[2].filename, palette.get()); // slot 6

		sounds.clear();
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 138
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[4].filename, app->archive.get())); // slot 139

		scene.clear();
	}
	else if (currentScene == 6) // Ship turning around to fight + a lot of fighting scenes
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1D.IFF", app->archive.get());

		std::string imagepak = movie->files[0].filename; // slot 0

		scenetext.clear();
		font.clear();
		sounds.clear();

		scene.clear();

		palette = LoadPakPalette(imagepak, 22);
		scene.push_back(LoadPakImage(imagepak, 23, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 24, palette.get()));

		palette = LoadPakPalette(imagepak, 25);
		scene.push_back(LoadPakImage(imagepak, 26, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 27, palette.get()));

		palette = LoadPakPalette(imagepak, 28);
		scene.push_back(LoadPakImage(imagepak, 29, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 30, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 31, palette.get()));

		palette = LoadPakPalette(imagepak, 32);
		scene.push_back(LoadPakImage(imagepak, 33, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 34, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 35, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 36, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 37, palette.get()));

		palette = LoadPakPalette(imagepak, 38);
		scene.push_back(LoadPakImage(imagepak, 39, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 40, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 41, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 42, palette.get()));

		palette = LoadPakPalette(imagepak, 43);
		scene.push_back(LoadPakImage(imagepak, 44, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 45, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 46, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 47, palette.get()));

		palette = LoadPakPalette(imagepak, 48);
		scene.push_back(LoadPakImage(imagepak, 49, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 50, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 51, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 52, palette.get()));

		palette = LoadPakPalette(imagepak, 53);
		scene.push_back(LoadPakImage(imagepak, 54, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 55, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 56, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 57, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 58, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 59, palette.get()));

		palette = LoadPakPalette(imagepak, 60);
		scene.push_back(LoadPakImage(imagepak, 61, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 62, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 63, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 64, palette.get()));
	}
	else if (currentScene == 7) // pirate conv 3
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1E1.IFF", app->archive.get());

		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		WCPak pak(textpak, app->archive.get());
		scenetext.clear();
		for (size_t i = 13; i < 16; i++)
		{
			std::vector<uint8_t> buf = pak.files[i];
			buf.push_back(0);
			scenetext.push_back((const char*)buf.data());
		}

		palette = LoadPakPalette(imagepak, 10);
		font = LoadShpImage(movie->files[2].filename, palette.get()); // slot 6

		sounds.clear();
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 94
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[4].filename, app->archive.get())); // slot 95

		scene.clear();
	}
	else if (currentScene == 8) // protagonist conv 3
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1E2.IFF", app->archive.get());
	}
	else if (currentScene == 9) // pirate conv 4
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1E3.IFF", app->archive.get());
	}
	else if (currentScene == 10) // protagonist conv 4
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1E4.IFF", app->archive.get());
	}
	else if (currentScene == 11) // alien aircraft gets hit by missile
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1F.IFF", app->archive.get());

		std::string imagepak = movie->files[0].filename; // slot 0

		scenetext.clear();
		font.clear();
		sounds.clear();

		WCPak pak2(imagepak, app->archive.get());
		scene.clear();

		palette = LoadPakPalette(imagepak, 65);
		scene.push_back(LoadPakImage(imagepak, 66, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 67, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 68, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 69, palette.get()));

		palette = LoadPakPalette(imagepak, 70);
		scene.push_back(LoadPakImage(imagepak, 71, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 72, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 73, palette.get()));

		palette = LoadPakPalette(imagepak, 74);
		scene.push_back(LoadPakImage(imagepak, 75, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 76, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 77, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 78, palette.get()));
		scene.push_back(LoadPakImage(imagepak, 79, palette.get()));
	}

	SetStartTime();
}

void IntroScreen::Render(RenderDevice* renderdev)
{
	int64_t time = GetMovieTime();

	if (currentScene == 0)
	{
		int cameraX = 0;

		int64_t scrollstart = 1000;
		int64_t scrollend = 1000 + 480 * 35;
		if (time > scrollstart && time < scrollend)
		{
			time -= scrollstart;
			cameraX = time / 35;

			int textindex = time / 4000;
			if (textindex < 4)
				DrawText(renderdev, 160, 172, scenetext[textindex], font, GameTextAlignment::Center);
		}
		else if (time >= scrollend)
		{
			cameraX = 480;

			// To do: play sound effect here - where does it come from?
		}

		renderdev->DrawImage(0 - cameraX, 32, scene[0][0].get());
		renderdev->DrawImage(320 - cameraX, 32, scene[1][0].get());
		renderdev->DrawImage(400 - cameraX, 32, scene[2][0].get());
		renderdev->DrawImage(480 - cameraX, 32, scene[3][0].get());

		if (time >= scrollend)
		{
			size_t frame = std::min((size_t)((time - scrollend) / 35), scene[4].size() - 1);
			renderdev->DrawImage(480 - cameraX, 32, scene[4][frame].get());

			if ((size_t)((time - scrollend) / 35) >= scene[4].size())
				SetScene(1);
		}
	}
	else if (currentScene == 1)
	{
		// To do: play sample when frame1 changes from 0 to 1 - where does it come from?

		size_t index = (size_t)(time / 35);
		if (index < scene[2].size())
		{
			size_t frame0 = std::min(index, scene[0].size() - 1);
			size_t frame1 = std::min(index, scene[1].size() - 1);
			size_t frame2 = std::min(index, scene[2].size() - 1);
			renderdev->DrawImage(0, 0, scene[0][frame0].get());
			renderdev->DrawImage(0, 0, scene[1][frame1].get());
			renderdev->DrawImage(0, 91, scene[2][frame2].get());
		}
		else
		{
			index -= scene[2].size();
			size_t frame3 = std::min(index, scene[3].size() - 1);
			size_t frame4 = std::min(index, scene[4].size() - 1);
			renderdev->DrawImage(0, 32, scene[3][frame3].get());
			renderdev->DrawImage(0, 32, scene[4][frame4].get());
			if (index >= scene[4].size())
				SetScene(2);
		}

	}
	else if (currentScene == 2)
	{
		int soundEnd0 = sounds[0]->samples.size() * 1000 / sounds[0]->samplerate + 250;
		int soundEnd1 = soundEnd0 + sounds[1]->samples.size() * 1000 / sounds[1]->samplerate + 250;
		int soundEnd2 = soundEnd1 + sounds[2]->samples.size() * 1000 / sounds[2]->samplerate + 250;
		if (time < soundEnd0)
		{
			DrawText(renderdev, 160, 172, scenetext[0], font, GameTextAlignment::Center);

			if (currentSound != 0)
			{
				currentSound = 0;
				PlaySound(sounds[currentSound].get());
			}
		}
		else if (time < soundEnd1)
		{
			DrawText(renderdev, 160, 172, scenetext[1], font, GameTextAlignment::Center);

			if (currentSound != 1)
			{
				currentSound = 1;
				PlaySound(sounds[currentSound].get());
			}
		}
		else if (time < soundEnd2)
		{
			DrawText(renderdev, 160, 172, scenetext[2], font, GameTextAlignment::Center);

			if (currentSound != 2)
			{
				currentSound = 2;
				PlaySound(sounds[currentSound].get());
			}
		}
		else
		{
			SetScene(3);
		}
	}
	else if (currentScene == 3)
	{
		int soundEnd0 = sounds[0]->samples.size() * 1000 / sounds[0]->samplerate + 250;
		int soundEnd1 = soundEnd0 + sounds[1]->samples.size() * 1000 / sounds[1]->samplerate + 250;
		if (time < soundEnd0)
		{
			DrawText(renderdev, 160, 172, scenetext[0], font, GameTextAlignment::Center);

			if (currentSound != 0)
			{
				currentSound = 0;
				PlaySound(sounds[currentSound].get());
			}
		}
		else if (time < soundEnd1)
		{
			DrawText(renderdev, 160, 172, scenetext[1], font, GameTextAlignment::Center);

			if (currentSound != 1)
			{
				currentSound = 1;
				PlaySound(sounds[currentSound].get());
			}
		}
		else
		{
			SetScene(4);
		}
	}
	else if (currentScene == 4)
	{
		SetScene(5);
	}
	else if (currentScene == 5)
	{
		SetScene(6);
	}
	else if (currentScene == 6)
	{
		size_t frame0 = std::min((size_t)(time / 35), scene[0].size() - 1);
		size_t frame1 = std::min((size_t)(time / 35), scene[1].size() - 1);
		renderdev->DrawImage(0, 32, scene[0][frame0].get());
		renderdev->DrawImage(0, 32, scene[1][frame1].get());
		if ((size_t)(time / 35) >= scene[1].size())
			SetScene(7);
	}
	else if (currentScene == 7)
	{
		SetScene(8);
	}
	else if (currentScene == 8)
	{
		SetScene(9);
	}
	else if (currentScene == 9)
	{
		SetScene(10);
	}
	else if (currentScene == 10)
	{
		SetScene(11);
	}
	else if (currentScene == 11)
	{
		size_t frame0 = std::min((size_t)(time / 35), scene[0].size() - 1);
		size_t frame1 = std::min((size_t)(time / 35), scene[1].size() - 1);
		size_t frame2 = std::min((size_t)(time / 35), scene[2].size() - 1);
		size_t frame3 = std::min((size_t)(time / 35), scene[3].size() - 1);
		renderdev->DrawImage(0, 32, scene[0][frame0].get());
		renderdev->DrawImage(0, 32, scene[1][frame1].get());
		renderdev->DrawImage(0, 32, scene[2][frame1].get());
		renderdev->DrawImage(0, 32, scene[3][frame1].get());

		ShowScreen(std::make_unique<MainMenuScreen>(app));
	}
}

void IntroScreen::OnKeyDown(InputKey key)
{
	if (key == InputKey::Escape)
	{
		ShowScreen(std::make_unique<MainMenuScreen>(app));
	}
}

static int64_t GetTimePoint()
{
	using namespace std::chrono;
	return (uint64_t)(duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count());
}

void IntroScreen::SetStartTime()
{
	StartTimeNS = GetTimePoint();
}

int64_t IntroScreen::GetMovieTime()
{
	if (StartTimeNS == 0)
		SetStartTime();

	uint64_t curTime = GetTimePoint();
	return (curTime - StartTimeNS) / 1'000'000;
}
