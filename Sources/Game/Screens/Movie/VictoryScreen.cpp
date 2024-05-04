
#include "VictoryScreen.h"
#include "FileFormat/WCMovie.h"
#include "Game/GameApp.h"

VictoryScreen::VictoryScreen(GameApp* app) : MovieScreen(app)
{
	movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\VICTORY1.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\VICTORY2.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\VICTORY3.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\VICTORY4.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\VICTORY5.IFF", app->archive.get());
	auto palette = LoadPakPalette("DATA\\MIDGAMES\\VICTORY.PAK", 0);
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\VICTORY.PAK", 1, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\VICTORY.PAK", 2, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\VICTORY.PAK", 3, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\VICTORY.PAK", 4, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\VICTORY.PAK", 5, palette.get()));
	offsetx = { 0, 0, 0, 0, 0 };
	offsety = { 0, 28, 85, 108, 129 };
}
