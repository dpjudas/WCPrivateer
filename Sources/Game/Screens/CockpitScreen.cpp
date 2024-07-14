
#include "CockpitScreen.h"
#include "FileFormat/WCCockpit.h"
#include "Game/GameApp.h"

CockpitScreen::CockpitScreen(GameApp* app) : GameScreen(app)
{
	//cockpit = std::make_unique<WCCockpit>("CLUNKCK", app->archive.get());
	//cockpit = std::make_unique<WCCockpit>("TUGCK", app->archive.get());
	//cockpit = std::make_unique<WCCockpit>("MERCHCK", app->archive.get());
	cockpit = std::make_unique<WCCockpit>("FIGHTCK", app->archive.get());
}

CockpitScreen::~CockpitScreen()
{
}

void CockpitScreen::Render(RenderDevice* renderdev)
{
	if (!palette)
	{
		palette = LoadSpacePalette();
		front = LoadWCImage(*cockpit->front.shape, palette.get());
	}

	framecounter++;

	GameTexture* frame = front[(framecounter / 20) % front.size()].get();
	renderdev->DrawImage(frame->x, frame->y, frame->width, frame->height, frame);
}
