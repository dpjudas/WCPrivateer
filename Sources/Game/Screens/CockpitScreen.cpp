
#include "CockpitScreen.h"
#include "NavScreen.h"
#include "FileFormat/WCCockpit.h"
#include "FileFormat/WCSpace.h"
#include "FileFormat/WCPak.h"
#include "FileFormat/FileEntryReader.h"
#include "Game/GameApp.h"
#include "Game/GameObjects/GameObject.h"

CockpitScreen::CockpitScreen(GameApp* app) : GameScreen(app)
{
	for (int i = 0; i < 100; i++)
	{
		Spawn<SpaceDebris>(app);
	}
	for (int i = 0; i < 16; i++)
	{
		Spawn<Asteroid>(app);
	}

	Spawn<StarBase>(app);

	app->player = Spawn<PlayerPawn>(app);
	app->player->target = Spawn<SpaceShip>(app);

	cockpit = std::make_unique<WCCockpit>(app->player->cockpit, app->archive.get());
	software = std::make_unique<WCCockpitSoftware>(cockpit->software, app->archive.get());
	misc = std::make_unique<WCCockpitMisc>(app->archive.get());
	plaques = std::make_unique<WCCockpitPlaques>(app->archive.get());

	starwhite = std::make_unique<WCSpaceSprite>("STARWHIT", app->archive.get());
	moon[0] = std::make_unique<WCSpaceSprite>("MOON1", app->archive.get());
	moon[1] = std::make_unique<WCSpaceSprite>("MOON2", app->archive.get());
	moon[2] = std::make_unique<WCSpaceSprite>("MOON3", app->archive.get());

	targeting = std::make_unique<WCTargetingType>(app->archive.get());
}

CockpitScreen::~CockpitScreen()
{
}

CockpitScreen::Sprite* CockpitScreen::getSprite(const std::string& name)
{
	if (name.empty())
		return nullptr;

	Sprite& sprite = spaceSprites[name];
	if (!sprite.sprite)
	{
		sprite.sprite = std::make_unique<WCSpaceSprite>(name, app->archive.get());
		if (sprite.sprite->shape)
			sprite.shape = LoadWCImage(*sprite.sprite->shape, palette.get());
		if (sprite.sprite->target)
			sprite.target = LoadWCImage(*sprite.sprite->target, palette.get());
	}
	return &sprite;
}

CockpitScreen::Ship* CockpitScreen::getShip(const std::string& name)
{
	if (name.empty())
		return nullptr;

	Ship& ship = spaceShips[name];
	if (!ship.ship)
	{
		ship.ship = std::make_unique<WCSpaceShip>(name, app->archive.get());
		for (auto& shape : ship.ship->shapes)
			ship.shapes.push_back(LoadWCImage(*shape, palette.get()));
		if (ship.ship->target)
			ship.target = LoadWCImage(*ship.ship->target, palette.get());
		if (ship.ship->weapon)
			ship.weapon = LoadWCImage(*ship.ship->weapon, palette.get());
		if (ship.ship->burn)
			ship.burn = LoadWCImage(*ship.ship->burn, palette.get());
	}
	return &ship;
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

		for (const auto& t : moon)
		{
			stars.push_back(LoadWCImage(*t->shape, palette.get()));
		}
		stars.push_back(LoadWCImage(*starwhite->shape, palette.get()));

		for (int i = 0; i < 25; i++)
		{
			quaternion rotation = quaternion::euler(radians(app->Random(0.0f, 360.0f)), radians(app->Random(0.0f, 360.0f)), 0.0f);
			StarLocation location;
			location.position = rotation * vec3(0.0f, 0.0f, 10000.0f);
			location.index = app->Random(0, (int)stars.size() - 1);
			starLocations.push_back(location);
		}

		crosshair = LoadWCImage(*cockpit->crosshair, palette.get());
		navigationCrosshair = LoadWCImage(*cockpit->navigationCrosshair, palette.get());
		itts = LoadWCImage(*cockpit->itts, palette.get());
		shield = LoadWCImage(*cockpit->shield, palette.get());
		fuel = LoadWCImage(*cockpit->fuel, palette.get());
		autopilot = LoadWCImage(*cockpit->autopilot, palette.get());
		energy = LoadWCImage(*cockpit->energy[0].shape, palette.get());

		font = LoadShpImage("DATA\\FONTS\\" + cockpit->font + ".SHP", palette.get());

		guns = LoadWCImage(*software->guns, palette.get());
		weapons = LoadWCImage(*software->weapons, palette.get());

		explosion = LoadWCImage(*misc->explosionShape, palette.get());
		plaquetextures = LoadWCImage(*plaques->shape, palette.get());

		blackTexture = std::make_unique<GameTexture>();
		blackTexture->x = 0;
		blackTexture->y = 0;
		blackTexture->width = 1;
		blackTexture->height = 1;
		blackTexture->pixels.resize(1, 0xff000000);

		radar = LoadWCImage(*targeting->targets.front().radarShape, palette.get());
	}

	for (auto& obj : app->gameObjects)
		obj->Tick(app->TimeElapsed);

	// Setup 3D viewport

	float viewportX = 0.0f;
	float viewportY = -20.0f;
	float viewportWidth = 320.0;
	float viewportHeight = 200.0;
	float halfViewportWidth = viewportWidth * 0.5f;
	float halfViewportHeight = viewportHeight * 0.5f;

	vec3 viewPos = app->player->position;
	quaternion viewRotation = app->player->rotation;

	mat4 worldToView = mat4::quaternion(inverse(viewRotation)) * mat4::translate(vec3(0.0f) - viewPos);
	mat4 projMatrix = mat4::perspective(50.0f, viewportWidth / viewportHeight, 1.0f, 65535.0f, handedness::left, clipzrange::zero_positive_w);

	// Draw space backdrop:

	for (const StarLocation& location : starLocations)
	{
		vec4 clippos = projMatrix * worldToView * vec4(location.position, 1.0f);
		if (clippos.w <= 0.0f)
			continue;

		clippos.w = 1.0f / clippos.w;
		clippos.x *= clippos.w;
		clippos.y *= clippos.w;
		clippos.z *= clippos.w;
		vec3 screenpos = { viewportX + (1.0f + clippos.x) * halfViewportWidth, viewportY + (1.0f - clippos.y) * halfViewportHeight, clippos.w };

		GameTexture* tex = stars[location.index][0].get();
		renderdev->DrawImage(screenpos.x, screenpos.y, tex);
	}

	// Game objects

	std::vector<std::pair<float, GameObject*>> sortedObjects;
	sortedObjects.reserve(app->gameObjects.size());
	for (auto& obj : app->gameObjects)
	{
		vec3 pos = obj->position - viewPos;
		float distSqr = dot(pos, pos);
		sortedObjects.emplace_back(distSqr, obj.get());
	}
	std::sort(sortedObjects.begin(), sortedObjects.end(), [](auto& a, auto& b) { return a.first > b.first; });

	for (auto& item : sortedObjects)
	{
		GameObject* obj = item.second;

		if (obj == app->player)
			continue;

		vec4 clippos = projMatrix * worldToView * vec4(obj->position, 1.0f);
		if (clippos.w <= 0.0f)
			continue;

		clippos.w = 1.0f / clippos.w;
		clippos.x *= clippos.w;
		clippos.y *= clippos.w;
		clippos.z *= clippos.w;
		vec3 screenpos = { viewportX + (1.0f + clippos.x) * halfViewportWidth, viewportY + (1.0f - clippos.y) * halfViewportHeight, clippos.w };

		float fade = std::min(screenpos.z * obj->size * 2.0f, 1.0f);

		if (Sprite* sprite = getSprite(obj->sprite))
		{
			for (auto& tex : sprite->shape)
			{
				float rotation = 0.0f;
				renderdev->Draw3DImage(screenpos.x, screenpos.y, screenpos.z * obj->size, rotation, tex.get(), fade, fade, fade);
			}
		}

		if (Ship* ship = getShip(obj->ship))
		{
			float rotation = 0.0f;
			int direction = 0; // 6 yaw * 6 pitch + 1 unknown. Needs mirroring for negative yaw
			renderdev->Draw3DImage(screenpos.x, screenpos.y, screenpos.z * obj->size, rotation, ship->shapes[direction].front().get(), fade, fade, fade);
		}
	}

	// Draw energy indicator:
	{
		auto& e = cockpit->energy.front();
		renderdev->DrawImage(e.x0, e.y0, e.x1 - e.x0 + 1, e.y1 - e.y0 + 1, blackTexture.get());
		renderdev->DrawImage((e.x0 + e.x1) / 2, (e.y0 + e.y1) / 2, energy[0].get());
	}

	// Draw fuel indicator:
	{
		auto& e = cockpit->fuelPos;
		renderdev->DrawImage(e.x0, e.y0, e.x1 - e.x0 + 1, e.y1 - e.y0 + 1, blackTexture.get());
		renderdev->DrawImage((e.x0 + e.x1) / 2, (e.y0 + e.y1) / 2, fuel[0].get());
	}

	// Draw autopilot indicator:
	{
		auto& e = cockpit->autoPos;
		renderdev->DrawImage(e.x0, e.y0, e.x1 - e.x0 + 1, e.y1 - e.y0 + 1, blackTexture.get());
		renderdev->DrawImage((e.x0 + e.x1) / 2, (e.y0 + e.y1) / 2, autopilot[1].get());
	}

	// Draw shield indicator:
	{
		auto& e = cockpit->shieldPos;
		renderdev->DrawImage(e.x0, e.y0, e.x1 - e.x0 + 1, e.y1 - e.y0 + 1, blackTexture.get());
		for (auto& image : shield)
		{
			renderdev->DrawImage((e.x0 + e.x1) / 2, (e.y0 + e.y1) / 2, image.get());
		}
	}

	// Draw radar:
	{
		auto& e = cockpit->radarPos;
		renderdev->DrawImage(e.x0, e.y0, e.x1 - e.x0 + 1, e.y1 - e.y0 + 1, blackTexture.get());
		renderdev->DrawImage((e.x0 + e.x1) / 2, (e.y0 + e.y1) / 2, radar[0].get());
	}

	// Draw console: (where does the 5,-20 offset come from?)
	{
		int offx = 5;
		int offy = -20;

		auto& e = cockpit->cmfd;
		renderdev->DrawImage(e.x0 + offx, e.y0 + offy, e.x1 - e.x0 + 1, e.y1 - e.y0 + 1, blackTexture.get());

		// To do: draw contents of the console here
		if (app->player->target)
		{
			Ship* ship = getShip(app->player->target->ship);
			for (auto& tex : ship->target)
				renderdev->DrawImage((e.x0 + e.x1) / 2 + offx, (e.y0 + e.y1) / 2 + offy, tex.get());
		}

		// renderdev->DrawImage(e.x0 + 16, e.y0 + 16, guns[0].get());
		// renderdev->DrawImage(e.x0 + 32, e.y0 + 16, weapons[0].get());
	}

	// Draw the cockpit image:
	renderdev->DrawImage(0, 0, front[0].get());

	// Draw speed: (where does the 60,38 offset come from?)
	DrawText(renderdev, 60 + cockpit->setSpeed.x, 38 + cockpit->setSpeed.y, cockpit->setSpeed.text + std::to_string(250), font);
	DrawText(renderdev, 60 + cockpit->actualSpeed.x, 38 + cockpit->actualSpeed.y, cockpit->actualSpeed.text + std::to_string(998), font);

	// Draw crosshair: (how does it know the location?)
	renderdev->DrawImage(160, 75, crosshair[0].get());

	// Where is this explosion used? what palette does it map to?
	/* {
		GameTexture* tex = explosion[(framecounter / 20) % explosion.size()].get();
		renderdev->DrawImage(160, 100, tex);
	}*/

	// Junk that was never used?
	/* {
		GameTexture* tex = plaquetextures[(framecounter / 20) % plaquetextures.size()].get();
		renderdev->DrawImage(160, 100, tex);
	} */
}

void CockpitScreen::OnKeyDown(InputKey key)
{
	if (key == InputKey::M)
	{
		PushScreen(std::make_unique<NavScreen>(app));
	}
	else if (key == InputKey::Left)
	{
		app->player->input.turnLeftPressed = true;
	}
	else if (key == InputKey::Right)
	{
		app->player->input.turnRightPressed = true;
	}
	else if (key == InputKey::Up)
	{
		app->player->input.turnUpPressed = true;
	}
	else if (key == InputKey::Down)
	{
		app->player->input.turnDownPressed = true;
	}
	else if (key == InputKey::Q)
	{
		app->player->input.bankLeftPressed = true;
	}
	else if (key == InputKey::E)
	{
		app->player->input.bankRightPressed = true;
	}
	else if (key == InputKey::A)
	{
		app->player->input.forwardPressed = true;
	}
	else if (key == InputKey::Z)
	{
		app->player->input.backwardPressed = true;
	}
}

void CockpitScreen::OnKeyUp(InputKey key)
{
	if (key == InputKey::Left)
	{
		app->player->input.turnLeftPressed = false;
	}
	else if (key == InputKey::Right)
	{
		app->player->input.turnRightPressed = false;
	}
	else if (key == InputKey::Up)
	{
		app->player->input.turnUpPressed = false;
	}
	else if (key == InputKey::Down)
	{
		app->player->input.turnDownPressed = false;
	}
	else if (key == InputKey::Q)
	{
		app->player->input.bankLeftPressed = false;
	}
	else if (key == InputKey::E)
	{
		app->player->input.bankRightPressed = false;
	}
	else if (key == InputKey::A)
	{
		app->player->input.forwardPressed = false;
	}
	else if (key == InputKey::Z)
	{
		app->player->input.backwardPressed = false;
	}
}
