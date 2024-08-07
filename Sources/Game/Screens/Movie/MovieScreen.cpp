
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
	static int framecounter = 0;
	framecounter++;

	int i = 0;
	for (auto& f : movies)
	{
		GameTexture* frame = f[(framecounter / 20) % f.size()].get();
		renderdev->DrawImage(offsetx[i], offsety[i], frame);
		i++;
	}
}

void MovieScreen::OnKeyDown(InputKey key)
{
}
