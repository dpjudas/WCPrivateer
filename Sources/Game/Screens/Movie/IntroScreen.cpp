
#include "IntroScreen.h"
#include "FileFormat/WCMovie.h"
#include "Game/GameApp.h"

IntroScreen::IntroScreen(GameApp* app) : MovieScreen(app)
{
	movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1A.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1B.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1C1.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1C2.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1C3.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1C4.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1D.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1E1.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1E2.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1E3.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1E4.IFF", app->archive.get());
	//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\MID1F.IFF", app->archive.get());
	auto palette = LoadPakPalette("DATA\\MIDGAMES\\MID1.PAK", 0);
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\MID1.PAK", 1, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\MID1.PAK", 2, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\MID1.PAK", 3, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\MID1.PAK", 4, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\MID1.PAK", 5, palette.get()));
	int dx = -960;
	offsetx = { 0 + dx, 320 + dx, 640 + dx, 960 + dx, 960 + dx };
	offsety = { 32, 32, 32, 32, 32 };
}
