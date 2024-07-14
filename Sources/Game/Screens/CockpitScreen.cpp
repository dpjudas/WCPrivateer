
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

		crosshair = LoadWCImage(*cockpit->crosshair, palette.get());
		navigationCrosshair = LoadWCImage(*cockpit->navigationCrosshair, palette.get());
		itts = LoadWCImage(*cockpit->itts, palette.get());
		shield = LoadWCImage(*cockpit->shield, palette.get());
		fuel = LoadWCImage(*cockpit->fuel, palette.get());
		autopilot = LoadWCImage(*cockpit->autopilot, palette.get());
		energy = LoadWCImage(*cockpit->energy[0].shape, palette.get());

		font = LoadShpImage("DATA\\FONTS\\" + cockpit->font + ".SHP", palette.get());

		blackTexture = std::make_unique<GameTexture>();
		blackTexture->x = 0;
		blackTexture->y = 0;
		blackTexture->width = 1;
		blackTexture->height = 1;
		blackTexture->pixels.resize(1, 0xff000000);
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

	// Draw energy indicator:
	{
		auto& e = cockpit->energy.front();
		renderdev->DrawImage(e.x0, e.y0, e.x1 - e.x0 + 1, e.y1 - e.y0 + 1, blackTexture.get());
		renderdev->DrawImage((e.x0 + e.x1) / 2 + energy[0]->x, (e.y0 + e.y1) / 2 + energy[0]->y, energy[0]->width, energy[0]->height, energy[0].get());
	}

	// Draw fuel indicator:
	{
		auto& e = cockpit->fuelPos;
		renderdev->DrawImage(e.x0, e.y0, e.x1 - e.x0 + 1, e.y1 - e.y0 + 1, blackTexture.get());
		renderdev->DrawImage((e.x0 + e.x1) / 2 + fuel[0]->x, (e.y0 + e.y1) / 2 + fuel[0]->y, fuel[0]->width, fuel[0]->height, fuel[0].get());
	}

	// Draw autopilot indicator:
	{
		auto& e = cockpit->autoPos;
		renderdev->DrawImage(e.x0, e.y0, e.x1 - e.x0 + 1, e.y1 - e.y0 + 1, blackTexture.get());
		renderdev->DrawImage((e.x0 + e.x1) / 2 + autopilot[1]->x, (e.y0 + e.y1) / 2 + autopilot[1]->y, autopilot[1]->width, autopilot[1]->height, autopilot[1].get());
	}

	// Draw shield indicator:
	{
		auto& e = cockpit->shieldPos;
		renderdev->DrawImage(e.x0, e.y0, e.x1 - e.x0 + 1, e.y1 - e.y0 + 1, blackTexture.get());
		for (auto& image : shield)
		{
			renderdev->DrawImage((e.x0 + e.x1) / 2 + image->x, (e.y0 + e.y1) / 2 + image->y, image->width, image->height, image.get());
		}
	}

	// Draw radar:
	{
		auto& e = cockpit->radarPos;
		renderdev->DrawImage(e.x0, e.y0, e.x1 - e.x0 + 1, e.y1 - e.y0 + 1, blackTexture.get());
		// renderdev->DrawImage((e.x0 + e.x1) / 2 + radar[0]->x, (e.y0 + e.y1) / 2 + radar[0]->y, radar[0]->width, radar[0]->height, radar[0].get());
	}

	// Draw console: (where does the 5,-20 offset come from?)
	{
		auto& e = cockpit->cmfd;
		renderdev->DrawImage(e.x0 + 5, e.y0 - 20, e.x1 - e.x0 + 1, e.y1 - e.y0 + 1, blackTexture.get());

		// To do: draw contents of the console here
	}

	// Draw the cockpit image:
	renderdev->DrawImage(front[0]->x, front[0]->y, front[0]->width, front[0]->height, front[0].get());

	// Draw speed: (where does the 60,38 offset come from?)
	DrawText(renderdev, 60 + cockpit->setSpeed.x, 38 + cockpit->setSpeed.y, cockpit->setSpeed.text + std::to_string(250), font);
	DrawText(renderdev, 60 + cockpit->actualSpeed.x, 38 + cockpit->actualSpeed.y, cockpit->actualSpeed.text + std::to_string(998), font);

	// Draw crosshair: (how does it know the location?)
	renderdev->DrawImage(160 + crosshair[0]->x, 75 + crosshair[0]->y, crosshair[0]->width, crosshair[0]->height, crosshair[0].get());

}
