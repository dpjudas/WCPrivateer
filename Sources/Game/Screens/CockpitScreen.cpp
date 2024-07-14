
#include "CockpitScreen.h"
#include "FileFormat/WCCockpit.h"
#include "FileFormat/WCPak.h"
#include "FileFormat/FileEntryReader.h"
#include "Game/GameApp.h"

CockpitScreen::CockpitScreen(GameApp* app) : GameScreen(app)
{
	//cockpit = std::make_unique<WCCockpit>("CLUNKCK", app->archive.get());
	//cockpit = std::make_unique<WCCockpit>("TUGCK", app->archive.get());
	//cockpit = std::make_unique<WCCockpit>("MERCHCK", app->archive.get());
	cockpit = std::make_unique<WCCockpit>("FIGHTCK", app->archive.get());
	trash[0] = std::make_unique<WCSpaceSprite>("TRASH1", app->archive.get());
	trash[1] = std::make_unique<WCSpaceSprite>("TRASH2", app->archive.get());
	trash[2] = std::make_unique<WCSpaceSprite>("TRASH3", app->archive.get());
	trash[3] = std::make_unique<WCSpaceSprite>("TRASH4", app->archive.get());
	astroids[0] = std::make_unique<WCSpaceSprite>("ASTROID1", app->archive.get());
	astroids[1] = std::make_unique<WCSpaceSprite>("ASTROID2", app->archive.get());
	starwhite = std::make_unique<WCSpaceSprite>("STARWHIT", app->archive.get());
	moon[0] = std::make_unique<WCSpaceSprite>("MOON1", app->archive.get());
	moon[1] = std::make_unique<WCSpaceSprite>("MOON2", app->archive.get());
	moon[2] = std::make_unique<WCSpaceSprite>("MOON3", app->archive.get());
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

		WCPak pak("DATA\\APPEARNC\\STARS.PAK", app->archive.get());
		for (size_t i = 0; i < pak.files.size(); i++)
		{
			FileEntryReader reader = pak.openFile(i);
			WCImage image(reader);
			stars.push_back(LoadWCImage(image, palette.get()));
		}

		for (const auto& t : trash)
		{
			sprites.push_back(LoadWCImage(*t->shape, palette.get()));
		}

		for (const auto& t : astroids)
		{
			sprites.push_back(LoadWCImage(*t->shape, palette.get()));
		}

		/*
		for (const auto& t : moon)
		{
			stars.push_back(LoadWCImage(*t->shape, palette.get()));
		}
		stars.push_back(LoadWCImage(*starwhite->shape, palette.get()));
		*/

		for (int i = 0; i < 25; i++)
		{
			StarLocation location;
			location.x = rand() * 320 / RAND_MAX;
			location.y = rand() * 150 / RAND_MAX;
			location.index = rand() * stars.size() / (RAND_MAX + 1);
			starLocations.push_back(location);
		}

		for (int i = 0; i < 20; i++)
		{
			SpriteLocation location;
			location.x = rand() * 320 / RAND_MAX;
			location.y = rand() * 150 / RAND_MAX;
			location.index = rand() * sprites.size() / (RAND_MAX + 1);
			location.scale = 0.05f + ((float)(rand() / (double)RAND_MAX)) * 0.1f;
			spriteLocations.push_back(location);
		}
	}

	framecounter++;

	// Draw space:

	for (const StarLocation& location : starLocations)
	{
		GameTexture* tex = stars[location.index][0].get();
		renderdev->DrawImage(location.x + tex->x, location.y + tex->y, tex->width, tex->height, tex);
	}

	for (const SpriteLocation& location : spriteLocations)
	{
		GameTexture* tex = sprites[location.index][(framecounter / 20) % sprites[location.index].size()].get();
		float fade = location.scale * 4.0f;
		renderdev->DrawImage(
			location.x + (int)std::round(tex->x * location.scale),
			location.y + (int)std::round(tex->y * location.scale),
			(int)std::round(tex->width * location.scale),
			(int)std::round(tex->height * location.scale),
			tex, fade, fade, fade);
	}

	// Draw the cockpit image:
	renderdev->DrawImage(front[0]->x, front[0]->y, front[0]->width, front[0]->height, front[0].get());

	// Draw the cockpit instruments:

	//GameTexture* frame = front[(framecounter / 20) % front.size()].get();
	//renderdev->DrawImage(frame->x, frame->y, frame->width, frame->height, frame);
}
