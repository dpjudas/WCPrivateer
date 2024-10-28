
#include "GameObject.h"

/////////////////////////////////////////////////////////////////////////////

void PlayerPawn::Init()
{
	//cockpit = "CLUNKCK";
	//cockpit = "TUGCK";
	//cockpit = "MERCHCK";
	cockpit = "FIGHTCK";
	ship = "FIGHTER";
	size = 200.0f;
}

void PlayerPawn::Tick(float timeElapsed)
{
	float yaw = 0.0f;
	float pitch = 0.0f;
	float bank = 0.0f;
	float rotspeed = 40.0f * timeElapsed;

	if (input.turnLeftPressed)
		yaw -= rotspeed;
	if (input.turnRightPressed)
		yaw += rotspeed;

	if (input.turnUpPressed)
		pitch += rotspeed;
	if (input.turnDownPressed)
		pitch -= rotspeed;

	if (input.bankLeftPressed)
		bank += rotspeed;
	if (input.bankRightPressed)
		bank -= rotspeed;

	rotation = normalize(rotation * quaternion::euler(radians(pitch), radians(yaw), radians(bank)));

	if (input.forwardPressed)
		setSpeed = std::min(setSpeed + 100.0f * timeElapsed, maxSpeed);
	else if (input.backwardPressed)
		setSpeed = std::max(setSpeed - 100.0f * timeElapsed, 0.0f);

	vec3 thrustDirection = rotation * vec3(0.0f, 0.0f, 1.0f);
	float targetSpeed = input.afterburnerPressed ? afterburnerSpeed : setSpeed;
	float enginePower = (input.afterburnerPressed ? 400.0f : 150.0f) * timeElapsed;

	vec3 targetVelocity = thrustDirection * targetSpeed;
	vec3 deltaVelocity = targetVelocity - velocity;
	float thrustNeeded = length(deltaVelocity);
	if (thrustNeeded > 0.0f)
	{
		float thrustApplied = std::min(thrustNeeded, enginePower);
		velocity += deltaVelocity * (thrustApplied / thrustNeeded);
	}

	position += velocity * (0.1f * timeElapsed);
}

/////////////////////////////////////////////////////////////////////////////

void SpaceShip::Init()
{
	ship = "DEMON";
	size = 200.0f;
	radar = RadarVisibility::neutral;
}

void SpaceShip::Tick(float timeElapsed)
{
}

/////////////////////////////////////////////////////////////////////////////

void SpaceDebris::Init()
{
	spriteIndex = app->Random(0, 100);
	sprite = "TRASH" + std::to_string(Random(1, 4));
	size = 2.0f;
	position.x = app->Random(-50.0f, 50.0f);
	position.y = app->Random(-50.0f, 50.0f);
	position.z = app->Random(-50.0f, 50.0f);
}

void SpaceDebris::Tick(float timeElapsed)
{
	vec3 playerpos = app->playsim.player->position;
	vec3 delta = position - playerpos;
	if (delta.x > 50.0f) position.x = playerpos.x + std::fmod(delta.x + 50.0f, 100.0f) - 50.0f;
	if (delta.x < -50.0f) position.x = playerpos.x - std::fmod(50.0f - delta.x, 100.0f) + 50.0f;
	if (delta.y > 50.0f) position.y = playerpos.y + std::fmod(delta.y + 50.0f, 100.0f) - 50.0f;
	if (delta.y < -50.0f) position.y = playerpos.y - std::fmod(50.0f - delta.y, 100.0f) + 50.0f;
	if (delta.z > 50.0f) position.z = playerpos.z + std::fmod(delta.z + 50.0f, 100.0f) - 50.0f;
	if (delta.z < -50.0f) position.z = playerpos.z - std::fmod(50.0f - delta.z, 100.0f) + 50.0f;
}

/////////////////////////////////////////////////////////////////////////////

void Asteroid::Init()
{
	spriteIndex = app->Random(0, 100);
	sprite = "ASTROID" + std::to_string(Random(1, 2));
	size = 200.0f;
	position.x = app->Random(-2000.0f, 2000.0f);
	position.y = app->Random(-2000.0f, 2000.0f);
	position.z = app->Random(-2000.0f, 2000.0f);
}

/////////////////////////////////////////////////////////////////////////////

void StarBase::Init()
{
	sprite = "REFINE";
	position = { 500.0f, 500.0f, 2000.0f };
	size = 1000.0;
	radar = RadarVisibility::friendly;
}

void StarBase::Tick(float timeElapsed)
{
	vec3 v = app->playsim.player->position - position;
	if (dot(v, v) < 2000.0 * 2000.0)
	{
		if (app->playsim.landingZone != this)
		{
			app->playsim.landingZone = this;
			app->playsim.landingStartTime = app->GetGameTime();
			app->PlayMusic("DATA\\SOUND\\COMBAT.GEN", 9); // What is the landing song index?
		}
	}
	else if (app->playsim.landingZone == this)
	{
		app->playsim.landingStartTime = 0; // Cancel landing
	}
}

/////////////////////////////////////////////////////////////////////////////

void JumpPoint::Init()
{
	sprite = "JUMP";
	spriteIndex = 0;
	size = 1000.0;
	alpha = 0.40f;
}

void JumpPoint::Tick(float timeElapsed)
{
	timeLeft -= timeElapsed;
	while (timeLeft < 0.0f)
	{
		spriteIndex++;
		timeLeft += 0.1f;
	}
}
