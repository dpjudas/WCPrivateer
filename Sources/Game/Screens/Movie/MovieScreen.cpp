
#include "Precomp.h"
#include "MovieScreen.h"
#include "FileFormat/WCMovie.h"
#include "Game/GameApp.h"

MovieScreen::MovieScreen(GameApp* app) : GameScreen(app)
{
}

MovieScreen::~MovieScreen()
{
}

void MovieScreen::Render(RenderDevice* renderdev)
{
	if (!movie)
	{
		//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\CUBICLE.IFF", app->archive.get());
		//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\JUMP.IFF", app->archive.get());
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
		//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\TAKEOFFS.IFF", app->archive.get());
		//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\VICTORY1.IFF", app->archive.get());
		//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\VICTORY2.IFF", app->archive.get());
		//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\VICTORY3.IFF", app->archive.get());
		//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\VICTORY4.IFF", app->archive.get());
		//movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\VICTORY5.IFF", app->archive.get());

		movie = std::make_unique<WCMovie>("DATA\\MIDGAMES\\LANDINGS.IFF", app->archive.get());

		auto palette = LoadPakPalette("DATA\\MIDGAMES\\LANDINGS.PAK", 0);
		frames[0] = LoadPakImage("DATA\\MIDGAMES\\LANDINGS.PAK", 1, palette.get());
		frames[1] = LoadPakImage("DATA\\MIDGAMES\\LTOBASES.PAK", 0, palette.get());
		frames[2] = LoadPakImage("DATA\\MIDGAMES\\LANDINGS.PAK", 2, palette.get());
	}

	static int framecounter = 0;
	framecounter++;

	int offsetx[] = { 0, 200, 100 };
	int offsety[] = { 0, 60, 80 };

	int i = 0;
	for (auto& f : frames)
	{
		GameTexture* frame = f[(framecounter / 20) % f.size()].get();
		renderdev->DrawImage(frame->x + offsetx[i], frame->y + offsety[i], frame->width, frame->height, frame);
		i++;
	}
}

void MovieScreen::OnKeyDown(InputKey key)
{
}
