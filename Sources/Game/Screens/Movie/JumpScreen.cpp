
#include "JumpScreen.h"
#include "FileFormat/WCMovie.h"
#include "Game/GameApp.h"

JumpScreen::JumpScreen(GameApp* app) : MovieScreen(app)
{
	movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\JUMP.IFF", app->archive.get());
	auto palette = LoadPakPalette("DATA\\MIDGAMES\\JUMP.PAK", 0);
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\JUMP.PAK", 1, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\JUMP.PAK", 2, palette.get()));
	movies.push_back(LoadPakImage("DATA\\MIDGAMES\\JUMP.PAK", 3, palette.get()));
	offsetx = { 0, 0, 0 };
	offsety = { 0, 32, 102 };
}
