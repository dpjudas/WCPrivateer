
#include "TakeoffScreen.h"
#include "FileFormat/WCMovie.h"
#include "Game/GameApp.h"

TakeoffScreen::TakeoffScreen(GameApp* app) : MovieScreen(app)
{
	movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\TAKEOFFS.IFF", app->archive.get());
	auto palette = LoadPakPalette("DATA\\MIDGAMES\\TAKEOFFS.PAK", 0);
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\TAKEOFFS.PAK", 1, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\LTOBASES.PAK", 0, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\TAKEOFFS.PAK", 2, palette.get()));
	offsetx = { 0, 70, 100 };
	offsety = { 0, 60, 60 };
}
