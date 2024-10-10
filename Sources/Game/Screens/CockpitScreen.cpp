
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

	const WCSector& sector = app->sectordata->sectors[app->playsim.sectorIndex];

	app->playsim.player = Spawn<PlayerPawn>(app);
	app->playsim.player->position = app->playsim.spawnPoint;

	for (const WCSectorJumpItem& jump : sector.jumps)
	{
		const WCSectorSphere& sphere = sector.spheres[jump.sphereIndex];
		auto obj = Spawn<JumpPoint>(app);
		obj->position = { (float)sphere.x, (float)sphere.y, (float)sphere.z };
		obj->radar = RadarVisibility::neutral;
	}

	for (const WCSectorBaseItem& base : sector.bases)
	{
		auto obj = Spawn<StarBase>(app);
		obj->position = { (float)base.x, (float)base.y, (float)base.z };
		obj->sprite = base.name;
		obj->radar = RadarVisibility::friendly;
	}

	for (const WCSectorEncounterItem& encounter : sector.encounters)
	{
		const WCSectorSphere& sphere = sector.spheres[encounter.sphereIndex];
		auto obj = Spawn<SpaceShip>(app);
		obj->ship = encounter.name;
		obj->radar = RadarVisibility::hostile;
		obj->position = { (float)sphere.x, (float)sphere.y, (float)sphere.z };
		obj->position.x += app->Random(-500.0f, 500.0f);
		obj->position.y += app->Random(-500.0f, 500.0f);
		obj->position.z += app->Random(-500.0f, 500.0f);
	}

	/*
	Spawn<StarBase>(app);

	app->playsim.player = Spawn<PlayerPawn>(app);
	app->playsim.player->target = Spawn<SpaceShip>(app);

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
	*/

	cockpit = std::make_unique<WCCockpit>(app->playsim.player->cockpit, app->archive.get());
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

		whiteTexture = std::make_unique<GameTexture>();
		whiteTexture->x = 0;
		whiteTexture->y = 0;
		whiteTexture->width = 1;
		whiteTexture->height = 1;
		whiteTexture->pixels.resize(1, 0xffffffff);

		radar = LoadWCImage(*targeting->targets.front().radarShape, palette.get());
		dots[0] = LoadDotImage(174, 174, 174);
		dots[1] = LoadDotImage(31, 51, 222);
		dots[2] = LoadDotImage(255, 103, 0);
	}

	for (auto& obj : app->playsim.gameObjects)
	{
		obj->Tick(app->TimeElapsed);
	}

	// Find target closest to player
	{
		GameObject* target = nullptr;
		float targetCosAngle = std::cos(radians(30.0f));
		float targetDistance = 10000.0f;
		vec3 viewPos = app->playsim.player->position;
		vec3 viewDir = app->playsim.player->rotation * vec3(0.0f, 0.0f, 1.0f);
		for (auto& obj : app->playsim.gameObjects)
		{
			if (obj.get() == app->playsim.player || obj->radar == RadarVisibility::hidden)
				continue;

			vec3 dir = obj->position - viewPos;
			float distancesqr = dot(dir, dir);
			if (distancesqr < 10000.0f * 10000.0f)
			{
				float distance = std::sqrt(distancesqr);
				dir /= distance;
				float cosAngle = dot(dir, viewDir);
				if (cosAngle > targetCosAngle && targetDistance > distance)
				{
					target = obj.get();
					targetCosAngle = cosAngle;
					targetDistance = distance;
				}
			}
		}
		if (target)
			app->playsim.player->target = target;
	}

	vec2 targetBoxTL = vec2(0.0f);
	vec2 targetBoxBR = vec2(0.0f);
	vec2 navPoint = vec2(-1000.0f, -1000.0f);

	// Setup 3D viewport

	float viewportX = 0.0f;
	float viewportY = -25.0f;
	float viewportWidth = 320.0;
	float viewportHeight = 200.0;
	float halfViewportWidth = viewportWidth * 0.5f;
	float halfViewportHeight = viewportHeight * 0.5f;

	vec3 viewPos = app->playsim.player->position;
	quaternion viewRotation = app->playsim.player->rotation;

	mat4 worldToView = mat4::quaternion(inverse(viewRotation)) * mat4::translate(vec3(0.0f) - viewPos);
	mat4 projMatrix = mat4::perspective(50.0f, viewportWidth / viewportHeight, 1.0f, 65535.0f, handedness::left, clipzrange::zero_positive_w);

	// Find navpoint location

	const WCSector& sector = app->sectordata->sectors[app->playsim.sectorIndex];
	for (const WCSectorSphere& sphere : sector.spheres)
	{
		if (sphere.argtype == 7 && sphere.arg2 == app->playsim.navpoint)
		{
			vec4 clippos = projMatrix * worldToView * vec4(sphere.x, sphere.y, sphere.z, 1.0f);
			if (clippos.w <= 0.0f)
				break;

			clippos.w = 1.0f / clippos.w;
			clippos.x *= clippos.w;
			clippos.y *= clippos.w;
			clippos.z *= clippos.w;
			vec3 screenpos = { viewportX + (1.0f + clippos.x) * halfViewportWidth, viewportY + (1.0f - clippos.y) * halfViewportHeight, clippos.w };
			navPoint = screenpos.xy();
		}
	}

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
	sortedObjects.reserve(app->playsim.gameObjects.size());
	for (auto& obj : app->playsim.gameObjects)
	{
		vec3 pos = obj->position - viewPos;
		float distSqr = dot(pos, pos);
		sortedObjects.emplace_back(distSqr, obj.get());
	}
	std::sort(sortedObjects.begin(), sortedObjects.end(), [](auto& a, auto& b) { return a.first > b.first; });

	for (auto& item : sortedObjects)
	{
		GameObject* obj = item.second;

		if (obj == app->playsim.player)
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
				bool first = true;
				for (auto& tex : sprite->shape)
				{
					float rotation = 0.0f;
					float scale = screenpos.z * obj->size;
					renderdev->Draw3DImage(screenpos.x, screenpos.y, scale, scale, rotation, tex.get(), fade, fade, fade);

					if (obj == app->playsim.player->target)
					{
						vec2 tl, br;
						renderdev->GetBounds(screenpos.x, screenpos.y, scale, scale, rotation, tex.get(), tl, br);
						if (first)
						{
							targetBoxTL = tl;
							targetBoxBR = br;
							first = false;
						}
						else
						{
							targetBoxTL.x = std::min(targetBoxTL.x, tl.x);
							targetBoxTL.y = std::min(targetBoxTL.y, tl.y);
							targetBoxBR.x = std::max(targetBoxBR.x, br.x);
							targetBoxBR.y = std::max(targetBoxBR.y, br.y);
						}
					}
				}
			}
			else
			{
				int index = obj->spriteIndex % sprite->shape.size();
				float rotation = 0.0f;
				float scale = screenpos.z * obj->size;
				renderdev->Draw3DImage(screenpos.x, screenpos.y, scale, scale, rotation, sprite->shape[index].get(), fade, fade, fade);

				if (obj == app->playsim.player->target)
					renderdev->GetBounds(screenpos.x, screenpos.y, scale, scale, rotation, sprite->shape[index].get(), targetBoxTL, targetBoxBR);
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

			if (obj == app->playsim.player->target)
				renderdev->GetBounds(screenpos.x, screenpos.y, scale * mirror, scale, rotation, ship->shapes[direction].front().get(), targetBoxTL, targetBoxBR);
		}
	}

	if (app->playsim.player->target)
	{
		int x0 = (int)std::round(targetBoxTL.x) - 1;
		int y0 = (int)std::round(targetBoxTL.y) - 1;
		int x3 = (int)std::round(targetBoxBR.x) + 1;
		int y3 = (int)std::round(targetBoxBR.y) + 1;
		int x1 = x0 + (x3 - x0) / 4;
		int x2 = x3 - (x3 - x0) / 4;
		int y1 = y0 + (y3 - y0) / 4;
		int y2 = y3 - (y3 - y0) / 4;
		
		renderdev->DrawImage(x0, y0, x1 - x0, 1, whiteTexture.get(), 0.5f, 0.5f, 0.5f);
		renderdev->DrawImage(x2, y0, x3 - x2, 1, whiteTexture.get(), 0.5f, 0.5f, 0.5f);
		renderdev->DrawImage(x0, y0, 1, y1 - y0, whiteTexture.get(), 0.5f, 0.5f, 0.5f);
		renderdev->DrawImage(x3, y0, 1, y1 - y0, whiteTexture.get(), 0.5f, 0.5f, 0.5f);

		renderdev->DrawImage(x0, y3, x1 - x0, 1, whiteTexture.get(), 0.5f, 0.5f, 0.5f);
		renderdev->DrawImage(x2, y3, x3 - x2 + 1, 1, whiteTexture.get(), 0.5f, 0.5f, 0.5f);
		renderdev->DrawImage(x0, y2, 1, y3 - y2, whiteTexture.get(), 0.5f, 0.5f, 0.5f);
		renderdev->DrawImage(x3, y2, 1, y3 - y2 + 1, whiteTexture.get(), 0.5f, 0.5f, 0.5f);
	}

	// Draw nav point:
	renderdev->DrawImage((int)std::round(navPoint.x), (int)std::round(navPoint.y), navigationCrosshair[0].get());

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

		for (auto& obj : app->playsim.gameObjects)
		{
			if (obj->radar == RadarVisibility::hidden || obj.get() == app->playsim.player->target)
				continue;

			vec3 L = obj->position - viewPos;
			float distsqr = dot(L, L);
			if (distsqr > 10000.0f * 10000.0f)
				continue;

			int color = 0;
			if (obj->radar == RadarVisibility::friendly)
				color = 1;
			else if (obj->radar == RadarVisibility::hostile)
				color = 2;

			vec3 p = inverse(viewRotation) * L;
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

		if (app->playsim.player->target)
		{
			auto obj = app->playsim.player->target;

			int color = 0;
			if (obj->radar == RadarVisibility::friendly)
				color = 1;
			else if (obj->radar == RadarVisibility::hostile)
				color = 2;

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
		if (app->playsim.player->target)
		{
			int x = (e.x0 + e.x1) / 2 + offx;
			int y = (e.y0 + e.y1) / 2 + offy;

			if (Sprite* sprite = getSprite(app->playsim.player->target->sprite))
			{
				for (auto& tex : sprite->target)
					renderdev->DrawImage(x, y, tex.get());
			}
			else if (Ship* ship = getShip(app->playsim.player->target->ship))
			{
				for (auto& tex : ship->target)
					renderdev->DrawImage(x, y, tex.get());
			}

			vec3 v = app->playsim.player->target->position - app->playsim.player->position;
			float dist = std::sqrt(dot(v, v));

			DrawText(renderdev, x, y + 32, "Unknown  " + std::to_string((int)std::round(dist)) + "m", font, GameTextAlignment::Center, 1);
		}

		// renderdev->DrawImage(e.x0 + 16, e.y0 + 16, guns[0].get());
		// renderdev->DrawImage(e.x0 + 32, e.y0 + 16, weapons[0].get());
	}

	// Draw the cockpit image:
	renderdev->DrawImage(0, 0, front[0].get());

	// Draw speed: (where does the 60,38 offset come from?)
	int setSpeed = (int)std::round(app->playsim.player->input.afterburnerPressed ? app->playsim.player->afterburnerSpeed : app->playsim.player->setSpeed);
	int actualSpeed = (int)std::round(length(app->playsim.player->velocity));
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
		app->playsim.player->input.forwardPressed = false;
		app->playsim.player->input.backwardPressed = false;
		app->playsim.player->input.bankLeftPressed = false;
		app->playsim.player->input.bankRightPressed = false;
		app->playsim.player->input.turnLeftPressed = false;
		app->playsim.player->input.turnRightPressed = false;
		app->playsim.player->input.turnUpPressed = false;
		app->playsim.player->input.turnDownPressed = false;
		app->playsim.player->input.afterburnerPressed = false;
		PushScreen(std::make_unique<NavScreen>(app));
	}
	else if (key == InputKey::Left)
	{
		app->playsim.player->input.turnLeftPressed = true;
	}
	else if (key == InputKey::Right)
	{
		app->playsim.player->input.turnRightPressed = true;
	}
	else if (key == InputKey::Up)
	{
		app->playsim.player->input.turnUpPressed = true;
	}
	else if (key == InputKey::Down)
	{
		app->playsim.player->input.turnDownPressed = true;
	}
	else if (key == InputKey::Q)
	{
		app->playsim.player->input.bankLeftPressed = true;
	}
	else if (key == InputKey::E)
	{
		app->playsim.player->input.bankRightPressed = true;
	}
	else if (key == InputKey::A)
	{
		app->playsim.player->input.forwardPressed = true;
	}
	else if (key == InputKey::Z)
	{
		app->playsim.player->input.backwardPressed = true;
	}
	else if (key == InputKey::Tab)
	{
		app->playsim.player->input.afterburnerPressed = true;
	}
}

void CockpitScreen::OnKeyUp(InputKey key)
{
	if (key == InputKey::Left)
	{
		app->playsim.player->input.turnLeftPressed = false;
	}
	else if (key == InputKey::Right)
	{
		app->playsim.player->input.turnRightPressed = false;
	}
	else if (key == InputKey::Up)
	{
		app->playsim.player->input.turnUpPressed = false;
	}
	else if (key == InputKey::Down)
	{
		app->playsim.player->input.turnDownPressed = false;
	}
	else if (key == InputKey::Q)
	{
		app->playsim.player->input.bankLeftPressed = false;
	}
	else if (key == InputKey::E)
	{
		app->playsim.player->input.bankRightPressed = false;
	}
	else if (key == InputKey::A)
	{
		app->playsim.player->input.forwardPressed = false;
	}
	else if (key == InputKey::Z)
	{
		app->playsim.player->input.backwardPressed = false;
	}
	else if (key == InputKey::Tab)
	{
		app->playsim.player->input.afterburnerPressed = false;
	}
}
