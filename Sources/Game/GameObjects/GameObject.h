#pragma once

#include "Math/Vec.h"
#include "Math/Quaternion.h"
#include "Game/GameApp.h"
#include <cmath>

class GameApp;

enum class RadarVisibility
{
	hidden,
	neutral,
	friendly,
	hostile
};

class GameObject
{
public:
	virtual void Init() {}
	virtual void Tick(float timeElapsed) {}

	float Random(float minval, float maxval) { return minval + (maxval - minval) * rand() / RAND_MAX; }
	int Random(int minval, int maxval) { return (int)std::round(Random((float)minval, (float)maxval)); }

	std::string sprite;
	float alpha = 1.0f;
	int spriteIndex = -1;
	std::string cockpit;
	std::string ship;

	vec3 position = vec3(0.0f);
	float size = 10.0f;
	quaternion rotation;

	RadarVisibility radar = RadarVisibility::hidden;

	struct
	{
		bool forwardPressed = false;
		bool backwardPressed = false;
		bool bankLeftPressed = false;
		bool bankRightPressed = false;
		bool turnLeftPressed = false;
		bool turnRightPressed = false;
		bool turnUpPressed = false;
		bool turnDownPressed = false;
		bool afterburnerPressed = false;
	} input;

	GameApp* app = nullptr;
};

class StarBase : public GameObject
{
public:
	void Init() override;
	void Tick(float timeElapsed) override;

	int baseIndex = 0;
};

class JumpPoint : public GameObject
{
public:
	void Init() override;
	void Tick(float timeElapsed) override;

	float timeLeft = 0.0f;
};

class PlayerPawn : public GameObject
{
public:
	void Init() override;
	void Tick(float timeElapsed) override;

	GameObject* target = nullptr;

	vec3 velocity = vec3(0.0f);
	float setSpeed = 100.0f;
	float maxSpeed = 350.0f;
	float afterburnerSpeed = 700.0f;
};

class SpaceShip : public GameObject
{
public:
	void Init() override;
	void Tick(float timeElapsed) override;
};

class SpaceDebris : public GameObject
{
public:
	void Init() override;
	void Tick(float timeElapsed) override;
};

class Asteroid : public GameObject
{
public:
	void Init() override;
};

template<typename T>
T* Spawn(GameApp* app)
{
	app->playsim.gameObjects.push_back(std::make_unique<T>());
	auto obj = static_cast<T*>(app->playsim.gameObjects.back().get());
	obj->app = app;
	obj->Init();
	return obj;
}
