
#include "GameObject.h"

/////////////////////////////////////////////////////////////////////////////

void PlayerPawn::Init()
{
	//cockpit = "CLUNKCK";
	//cockpit = "TUGCK";
	//cockpit = "MERCHCK";
	cockpit = "FIGHTCK";
}

void PlayerPawn::Tick(float timeElapsed)
{
	float yaw = 0.0f;
	float pitch = 0.0f;
	float bank = 0.0f;
	float rotspeed = 40.0f * timeElapsed;
	float movspeed = 10.0f * timeElapsed;

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
		position += rotation * vec3(0.0f, 0.0f, movspeed);
	if (input.backwardPressed)
		position -= rotation * vec3(0.0f, 0.0f, movspeed);
}

/////////////////////////////////////////////////////////////////////////////

void SpaceShip::Init()
{
	// ship = "FIGHTER";
	// ship = "FRIGATE";
	ship = "DEMON";
	position = { 0.0f, 0.0f, 100.0f };
	size = 10.0;
}

void SpaceShip::Tick(float timeElapsed)
{
}

/////////////////////////////////////////////////////////////////////////////

void SpaceDebris::Init()
{
	sprite = "TRASH" + std::to_string(Random(1, 4));
	size = 2.0f;
	position.x = app->Random(-100.0f, 100.0f);
	position.y = app->Random(-100.0f, 100.0f);
	position.z = app->Random(-100.0f, 100.0f);
}

/////////////////////////////////////////////////////////////////////////////

void Asteroid::Init()
{
	sprite = "ASTROID" + std::to_string(Random(1, 2));
	size = 50.0f;
	position.x = app->Random(-100.0f, 100.0f);
	position.y = app->Random(-100.0f, 100.0f);
	position.z = app->Random(-100.0f, 100.0f);
}

/////////////////////////////////////////////////////////////////////////////

void StarBase::Init()
{
	sprite = "REFINE";
	position = { 500.0f, 500.0f, 2000.0f };
	size = 1000.0;
}
