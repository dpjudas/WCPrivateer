
#include "LandingScreen.h"
#include "FileFormat/WCMovie.h"
#include "Game/GameApp.h"
#include "Game/Screens/Scene/SceneScreen.h"

LandingScreen::LandingScreen(GameApp* app) : GameScreen(app)
{
	movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\LANDINGS.IFF", app->archive.get());
	palette = LoadPakPalette("DATA\\MIDGAMES\\LANDINGS.PAK", 0);
	background = LoadPakImage("DATA\\MIDGAMES\\LANDINGS.PAK", 1, palette.get());
	base = LoadPakImage("DATA\\MIDGAMES\\LTOBASES.PAK", 0, palette.get());
	ship = LoadPakImage("DATA\\MIDGAMES\\LANDINGS.PAK", 2, palette.get());

	startTime = app->GetGameTime();
}

LandingScreen::~LandingScreen()
{
}

void LandingScreen::Render(RenderDevice* renderdev)
{
	curTime = (int)(app->GetGameTime() - startTime);
	int framecounter = curTime / 12;

	int planetX = curTime / 34 - 100;

	// Background
	{
		GameTexture* frame = background[(framecounter / 20) % background.size()].get();
		renderdev->DrawImage(0, 0, frame);
	}

	// Planet / base
	{
		GameTexture* frame = base[(framecounter / 20) % base.size()].get();
		renderdev->DrawImage(planetX, 60, frame);
	}

	// Ship
	{
		GameTexture* frame = ship[std::min(framecounter / 20, (int)ship.size() - 1)].get();
		renderdev->DrawImage(100, 60, frame);
	}

	if (framecounter / 20 >= (int)ship.size())
	{
		ShowScreen(std::make_unique<SceneScreen>(app));
	}
}
