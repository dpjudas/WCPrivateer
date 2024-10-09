
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
	for (int i = 0; i < 1000; i++)
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

	auto ship2 = Spawn<SpaceShip>(app);
	ship2->ship = "FRIGATE";
	ship2->position = { 4.0f, -2.5f, 120.0f };
	ship2->size = 30.0f;
	ship2->rotation = quaternion::euler(30.0f, 10.0f, 0.0f);
	ship2->radar = RadarVisibility::friendly;

	auto ship3 = Spawn<SpaceShip>(app);
	ship3->ship = "FIGHTER";
	ship3->position = { -4.0f, 1.5f, 95.0f };
	ship2->radar = RadarVisibility::hostile;

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

		for (int i = 0; i < 250; i++)
		{
			quaternion rotation = quaternion::euler(radians(app->Random(0.0f, 360.0f)), radians(app->Random(0.0f, 360.0f)), 0.0f);
			StarLocation location;
			location.position = rotation * vec3(0.0f, 0.0f, 10000.0f);
			location.index = app->Random(0, (int)stars.size() - 1);
			starLocations.push_back(location);
		}

		for (const auto& t : moon)
		{
			quaternion rotation = quaternion::euler(radians(app->Random(0.0f, 360.0f)), radians(app->Random(0.0f, 360.0f)), 0.0f);
			StarLocation location;
			location.position = rotation * vec3(0.0f, 0.0f, 10000.0f);
			location.index = (int)stars.size();
			starLocations.push_back(location);
			stars.push_back(LoadWCImage(*t->shape, palette.get()));
		}

		{
			quaternion rotation = quaternion::euler(radians(app->Random(0.0f, 360.0f)), radians(app->Random(0.0f, 360.0f)), 0.0f);
			StarLocation location;
			location.position = rotation * vec3(0.0f, 0.0f, 10000.0f);
			location.index = (int)stars.size();
			starLocations.push_back(location);
			stars.push_back(LoadWCImage(*starwhite->shape, palette.get()));
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
		dots[0] = LoadDotImage(174, 174, 174);
		dots[1] = LoadDotImage(255, 103, 0);
		dots[2] = LoadDotImage(31, 51, 222);
	}

	for (auto& obj : app->gameObjects)
		obj->Tick(app->TimeElapsed);

	// Setup 3D viewport

	float viewportX = 0.0f;
	float viewportY = -25.0f;
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
		vec4 clippos = projMatrix * worldToView * vec4(viewPos + location.position, 1.0f);
		if (clippos.w <= 0.0f)
			continue;

		clippos.w = 1.0f / clippos.w;
		clippos.x *= clippos.w;
		clippos.y *= clippos.w;
		clippos.z *= clippos.w;
		vec3 screenpos = { viewportX + (1.0f + clippos.x) * halfViewportWidth, viewportY + (1.0f - clippos.y) * halfViewportHeight, clippos.w };

		GameTexture* tex = stars[location.index][0].get();
		renderdev->Draw3DImage(screenpos.x, screenpos.y, 1.0f, 1.0f, 0.0f, tex);
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
			if (obj->spriteIndex == -1)
			{
				for (auto& tex : sprite->shape)
				{
					float rotation = 0.0f;
					float scale = screenpos.z * obj->size;
					renderdev->Draw3DImage(screenpos.x, screenpos.y, scale, scale, rotation, tex.get(), fade, fade, fade);
				}
			}
			else
			{
				int index = obj->spriteIndex % sprite->shape.size();
				float rotation = 0.0f;
				float scale = screenpos.z * obj->size;
				renderdev->Draw3DImage(screenpos.x, screenpos.y, scale, scale, rotation, sprite->shape[index].get(), fade, fade, fade);
			}
		}

		if (Ship* ship = getShip(obj->ship))
		{
			vec3 p = normalize(inverse(obj->rotation) * (viewPos - obj->position));
			float latitude = degrees(std::acos(p.y)) - 90.0f;
			float longitude = degrees(std::atan2(p.x, p.z));

			while (longitude < 0.0f) longitude += 360.0f;
			while (longitude >= 360.0f) longitude -= 360.0f;

			// 0 to 6 range, mirrored for other side
			int yawindex = clamp((int)std::round(longitude * 6.0f / 180.0f), 0, 12);
			float mirror = 1.0f;
			if (yawindex > 6)
			{
				yawindex = 12 - yawindex;
				mirror = -1.0f;
			}

			// 0 to 4 range
			int pitchindex = clamp(2 + (int)std::round(latitude * 2.0f / 90.0f), 0, 4);

			float rotation = 0.0f;
			int direction = 1 + yawindex + pitchindex * 7;
			float scale = screenpos.z * obj->size;
			renderdev->Draw3DImage(screenpos.x, screenpos.y, scale * mirror, scale, rotation, ship->shapes[direction].front().get(), fade, fade, fade);
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

		int radarX = (e.x0 + e.x1) / 2;
		int radarY = (e.y0 + e.y1) / 2;
		int radarSize = 15;

		renderdev->DrawImage(radarX, radarY, radar[0].get());

		for (auto& obj : app->gameObjects)
		{
			if (obj->radar == RadarVisibility::hidden || obj.get() == app->player->target)
				continue;

			int color = 0;
			if (obj->radar == RadarVisibility::friendly)
				color = 2;
			else if (obj->radar == RadarVisibility::hostile)
				color = 1;

			vec3 p = inverse(viewRotation) * (obj->position - viewPos);
			float xangle = std::atan2(p.x, p.z);
			float yangle = std::atan2(-p.y, p.z);
			float xx = degrees(xangle) / 180.0f;
			float yy = degrees(yangle) / 180.0f;
			float lensqr = xx * xx + yy * yy;
			if (lensqr > 1.0f)
			{
				float rcplen = 1.0f / std::sqrt(lensqr);
				xx *= rcplen;
				yy *= rcplen;
			}
			int x = radarX + (int)std::round(xx * radarSize);
			int y = radarY + (int)std::round(yy * radarSize);
			renderdev->DrawImage(x, y, dots[color][0].get());
		}

		if (app->player->target)
		{
			auto obj = app->player->target;

			int color = 0;
			if (obj->radar == RadarVisibility::friendly)
				color = 2;
			else if (obj->radar == RadarVisibility::hostile)
				color = 1;

			vec3 p = inverse(viewRotation) * (obj->position - viewPos);
			float xangle = std::atan2(p.x, p.z);
			float yangle = std::atan2(-p.y, p.z);
			float xx = degrees(xangle) / 180.0f;
			float yy = degrees(yangle) / 180.0f;
			float lensqr = xx * xx + yy * yy;
			if (lensqr > 1.0f)
			{
				float rcplen = 1.0f / std::sqrt(lensqr);
				xx *= rcplen;
				yy *= rcplen;
			}
			int x = radarX + (int)std::round(xx * radarSize);
			int y = radarY + (int)std::round(yy * radarSize);

			renderdev->DrawImage(x - 1, y, dots[color][0].get());
			renderdev->DrawImage(x + 1, y, dots[color][0].get());
			renderdev->DrawImage(x, y - 1, dots[color][0].get());
			renderdev->DrawImage(x, y + 1, dots[color][0].get());
			renderdev->DrawImage(x, y, dots[color][0].get());
		}
	}

	// Draw console: (where does the 5,-20 offset come from?)
	{
		int offx = 6;
		int offy = -26;

		auto& e = cockpit->cmfd;
		renderdev->DrawImage(e.x0, e.y0 - 28, e.x1 - e.x0 + 16, e.y1 - e.y0 + 28, blackTexture.get());

		// To do: draw contents of the console here
		if (app->player->target)
		{
			int x = (e.x0 + e.x1) / 2 + offx;
			int y = (e.y0 + e.y1) / 2 + offy;

			Ship* ship = getShip(app->player->target->ship);
			for (auto& tex : ship->target)
				renderdev->DrawImage(x, y, tex.get());

			DrawText(renderdev, x, y + 32, "Unknown  17072m", font, GameTextAlignment::Center, 1);
		}

		// renderdev->DrawImage(e.x0 + 16, e.y0 + 16, guns[0].get());
		// renderdev->DrawImage(e.x0 + 32, e.y0 + 16, weapons[0].get());
	}

	// Draw the cockpit image:
	renderdev->DrawImage(0, 0, front[0].get());

	// Draw speed: (where does the 60,38 offset come from?)
	int setSpeed = (int)std::round(app->player->input.afterburnerPressed ? app->player->afterburnerSpeed : app->player->setSpeed);
	int actualSpeed = (int)std::round(length(app->player->velocity));
	DrawText(renderdev, 60 + cockpit->setSpeed.x, 38 + cockpit->setSpeed.y, cockpit->setSpeed.text + std::to_string(setSpeed), font);
	DrawText(renderdev, 60 + cockpit->actualSpeed.x, 38 + cockpit->actualSpeed.y, cockpit->actualSpeed.text + std::to_string(actualSpeed), font);

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
	if (key == InputKey::N)
	{
		app->player->input.forwardPressed = false;
		app->player->input.backwardPressed = false;
		app->player->input.bankLeftPressed = false;
		app->player->input.bankRightPressed = false;
		app->player->input.turnLeftPressed = false;
		app->player->input.turnRightPressed = false;
		app->player->input.turnUpPressed = false;
		app->player->input.turnDownPressed = false;
		app->player->input.afterburnerPressed = false;
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
	else if (key == InputKey::Tab)
	{
		app->player->input.afterburnerPressed = true;
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
	else if (key == InputKey::Tab)
	{
		app->player->input.afterburnerPressed = false;
	}
}
