
#include "LandingScreen.h"
#include "FileFormat/WCMovie.h"
#include "Game/GameApp.h"

LandingScreen::LandingScreen(GameApp* app) : MovieScreen(app)
{
	movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\LANDINGS.IFF", app->archive.get());
	auto palette = LoadPakPalette("DATA\\MIDGAMES\\LANDINGS.PAK", 0);
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\LANDINGS.PAK", 1, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\LTOBASES.PAK", 0, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\LANDINGS.PAK", 2, palette.get()));
	offsetx = { 0, 200, 100 };
	offsety = { 0, 60, 80 };
}
