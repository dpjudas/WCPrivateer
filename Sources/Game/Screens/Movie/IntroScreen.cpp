
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

void IntroScreen::AddScene(int start, int count, int palindex, std::string imagepak)
{
	auto palette = LoadPakPalette(imagepak, palindex);
	IntroScene scene;
	for (int i = 0; i < count; i++)
	{
		scene.images.push_back(LoadPakImage(imagepak, start + i, palette.get()));
		scene.x.push_back(0);
		scene.y.push_back(0);
	}
	scenes.push_back(std::move(scene));
}

void IntroScreen::AddText(int start, int count, std::string textpak)
{
	WCPak pak(textpak, app->archive.get());
	for (size_t i = 0; i < count; i++)
	{
		std::vector<uint8_t> buf = pak.files[start + i];
		buf.push_back(0);
		scenetext.push_back((const char*)buf.data());
	}
}

void IntroScreen::AddPirateScene(std::string imagepak, std::string fontname)
{
	auto facepal = LoadPakPalette(imagepak, 15);
	auto bgpal = LoadPakPalette(imagepak, 19);
	WCPalette palette;
	for (WCPalette* p : { facepal.get(), bgpal.get() })
	{
		for (int i = p->start, end = p->start + p->count; i < end; i++)
		{
			palette.palette[i] = p->palette[i];
		}
	}

	font = LoadShpImage(fontname, &palette); // slot 6
	background = LoadPakImage(imagepak, 20, &palette);
	face = LoadPakImage(imagepak, 16, &palette);

	faceX = 40;
	faceY = 38;
}

void IntroScreen::AddProtagonistScene(std::string imagepak, std::string fontname)
{
	auto facepal = LoadPakPalette(imagepak, 13);
	auto bgpal = LoadPakPalette(imagepak, 17);
	WCPalette palette;
	for (WCPalette* p : { facepal.get(), bgpal.get() })
	{
		for (int i = p->start, end = p->start + p->count; i < end; i++)
		{
			palette.palette[i] = p->palette[i];
		}
	}

	font = LoadShpImage(fontname, &palette); // slot 6
	background = LoadPakImage(imagepak, 18, &palette);
	face = LoadPakImage(imagepak, 14, &palette);

	faceX = 0;
	faceY = 37;
}

void IntroScreen::SetScene(int index)
{
	currentScene = index;
	currentSound = -1;
	scenetext.clear();
	font.clear();
	sounds.clear();
	scenes.clear();

	if (currentScene == 0) // Planet panning scene
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1A.IFF", app->archive.get());
		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		AddText(0, 4, textpak);
		AddScene(1, 5, 0, imagepak);
		font = LoadShpImage(movie->files[2].filename, LoadPakPalette(imagepak, 0).get()); // slot 2
		PlayMusic(movie->files[3].filename + ".GEN", 0); // slot 4
	}
	else if (currentScene == 1) // cockpit + ship chase scene
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1B.IFF", app->archive.get());
		std::string imagepak = movie->files[0].filename; // slot 0

		AddScene(7, 3, 6, imagepak);
		AddScene(11, 2, 10, imagepak);

		scenes[0].y[2] = 91;
		scenes[1].y[0] = 32;
		scenes[1].y[1] = 32;
	}
	else if (currentScene == 2) // pirate conversation
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1C1.IFF", app->archive.get());
		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		AddPirateScene(imagepak, movie->files[2].filename);
		AddText(4, 3, textpak);
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 61
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[4].filename, app->archive.get())); // slot 62
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[5].filename, app->archive.get())); // slot 63
	}
	else if (currentScene == 3) // protagonist replying back conversation
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1C2.IFF", app->archive.get());
		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		AddProtagonistScene(imagepak, movie->files[2].filename);
		AddText(7, 2, textpak);
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 94
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[4].filename, app->archive.get())); // slot 95
	}
	else if (currentScene == 4) // pirate conv 2
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1C3.IFF", app->archive.get());
		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		AddPirateScene(imagepak, movie->files[2].filename);
		AddText(9, 2, textpak);
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 116
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[4].filename, app->archive.get())); // slot 117
	}
	else if (currentScene == 5) // protagonist conv 2
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1C4.IFF", app->archive.get());
		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		AddProtagonistScene(imagepak, movie->files[2].filename);
		AddText(11, 2, textpak);
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 138
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[4].filename, app->archive.get())); // slot 139
	}
	else if (currentScene == 6) // Ship turning around to fight + a lot of fighting scenes
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1D.IFF", app->archive.get());
		std::string imagepak = movie->files[0].filename; // slot 0

		AddScene(23, 2, 22, imagepak);
		AddScene(26, 2, 25, imagepak);
		AddScene(29, 3, 28, imagepak);
		AddScene(33, 5, 32, imagepak);
		AddScene(39, 4, 38, imagepak);
		AddScene(44, 4, 43, imagepak);
		AddScene(49, 4, 48, imagepak);
		AddScene(54, 6, 53, imagepak);
		AddScene(61, 4, 60, imagepak);

		for (auto& scene : scenes)
		{
			for (auto& y : scene.y)
				y = 32;
		}
	}
	else if (currentScene == 7) // pirate conv 3
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1E1.IFF", app->archive.get());
		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		AddPirateScene(imagepak, movie->files[2].filename);
		AddText(13, 2, textpak);
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 94
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[4].filename, app->archive.get())); // slot 95
	}
	else if (currentScene == 8) // protagonist conv 3
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1E2.IFF", app->archive.get());
		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		AddProtagonistScene(imagepak, movie->files[2].filename);
		AddText(15, 1, textpak);
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 94
	}
	else if (currentScene == 9) // pirate conv 4
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1E3.IFF", app->archive.get());
		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		AddPirateScene(imagepak, movie->files[2].filename);
		AddText(16, 2, textpak);
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 95
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[4].filename, app->archive.get())); // slot 96
	}
	else if (currentScene == 10) // protagonist conv 4
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1E4.IFF", app->archive.get());
		std::string imagepak = movie->files[0].filename; // slot 0
		std::string textpak = movie->files[1].filename; // slot 1

		AddProtagonistScene(imagepak, movie->files[2].filename);
		AddText(18, 3, textpak);
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[3].filename, app->archive.get())); // slot 94
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[4].filename, app->archive.get())); // slot 95
		sounds.push_back(std::make_unique<WCVOCSound>(movie->files[5].filename, app->archive.get())); // slot 96
	}
	else if (currentScene == 11) // alien aircraft gets hit by missile
	{
		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1F.IFF", app->archive.get());
		std::string imagepak = movie->files[0].filename; // slot 0

		AddScene(66, 4, 65, imagepak);
		AddScene(71, 3, 70, imagepak);
		AddScene(75, 5, 74, imagepak);

		for (auto& scene : scenes)
		{
			for (auto& y : scene.y)
				y = 32;
		}
	}

	SetStartTime();
}

void IntroScreen::RenderConversationScene(RenderDevice* renderdev, int nextScene)
{
	int64_t time = GetMovieTime();

	renderdev->DrawImage(0, 0, background[0].get());
	renderdev->DrawImage(faceX, faceY, face[0].get()); // head
	renderdev->DrawImage(faceX, faceY, face[1].get()); // ?
	renderdev->DrawImage(faceX, faceY, face[2].get()); // eyes (2-6)
	renderdev->DrawImage(faceX, faceY, face[7].get()); // ? (7-15)
	renderdev->DrawImage(faceX, faceY, face[17].get()); // mouth (16-27)
	renderdev->DrawImage(faceX, faceY, face[28].get()); // body

	int i = 0;
	for (auto& sound : sounds)
	{
		int soundEnd = sound->samples.size() * 1000 / sound->samplerate + 250;
		if (time < soundEnd)
		{
			DrawText(renderdev, 160, 172, scenetext[i], font, GameTextAlignment::Center);

			if (currentSound != i)
			{
				currentSound = i;
				PlaySound(sounds[currentSound].get());
			}
			return;
		}
		time -= soundEnd;
		i++;
	}

	SetScene(nextScene);
}

void IntroScreen::RenderMovieScenes(RenderDevice* renderdev, int nextScene)
{
	int64_t time = GetMovieTime();
	for (auto& scene : scenes)
	{
		size_t index = (size_t)(time / 35);
		size_t endindex = 0;
		for (auto& image : scene.images)
			endindex = std::max(endindex, image.size());

		if (index < endindex)
		{
			int i = 0;
			for (auto& image : scene.images)
			{
				size_t frame = std::min(index, image.size() - 1);
				renderdev->DrawImage(scene.x[i], scene.y[i], scene.images[i][frame].get());
				i++;
			}
			return;
		}
		time -= endindex * 35;
	}

	SetScene(nextScene);
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

		auto& scene = scenes[0];
		renderdev->DrawImage(0 - cameraX, 32, scene.images[0][0].get());
		renderdev->DrawImage(320 - cameraX, 32, scene.images[1][0].get());
		renderdev->DrawImage(400 - cameraX, 32, scene.images[2][0].get());
		renderdev->DrawImage(480 - cameraX, 32, scene.images[3][0].get());

		if (time >= scrollend)
		{
			size_t frame = std::min((size_t)((time - scrollend) / 35), scene.images[4].size() - 1);
			renderdev->DrawImage(480 - cameraX, 32, scene.images[4][frame].get());

			if ((size_t)((time - scrollend) / 35) >= scene.images[4].size())
				SetScene(1);
		}
	}
	else if (currentScene == 1)
	{
		RenderMovieScenes(renderdev, 2);
	}
	else if (currentScene == 2)
	{
		RenderConversationScene(renderdev, 3);
	}
	else if (currentScene == 3)
	{
		RenderConversationScene(renderdev, 4);
	}
	else if (currentScene == 4)
	{
		RenderConversationScene(renderdev, 5);
	}
	else if (currentScene == 5)
	{
		RenderConversationScene(renderdev, 6);
	}
	else if (currentScene == 6)
	{
		RenderMovieScenes(renderdev, 7);
	}
	else if (currentScene == 7)
	{
		RenderConversationScene(renderdev, 8);
	}
	else if (currentScene == 8)
	{
		RenderConversationScene(renderdev, 9);
	}
	else if (currentScene == 9)
	{
		RenderConversationScene(renderdev, 10);
	}
	else if (currentScene == 10)
	{
		RenderConversationScene(renderdev, 11);
	}
	else if (currentScene == 11)
	{
		RenderMovieScenes(renderdev, 12);
	}
	else if (currentScene == 12)
	{
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
