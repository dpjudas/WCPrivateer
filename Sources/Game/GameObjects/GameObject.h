#pragma once

#include "Math/Vec.h"
#include "Math/Quaternion.h"
#include "Game/GameApp.h"
#include <cmath>

class GameApp;

class GameObject
{
public:
	virtual void Init() {}
	virtual void Tick(float timeElapsed) {}

	float Random(float minval, float maxval) { return minval + (maxval - minval) * rand() / RAND_MAX; }
	int Random(int minval, int maxval) { return (int)std::round(Random((float)minval, (float)maxval)); }

	std::string sprite;
	std::string cockpit;
	std::string ship;

	vec3 position = vec3(0.0f);
	float size = 10.0f;
	quaternion rotation;

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
	} input;

	GameApp* app = nullptr;
};

class StarBase : public GameObject
{
public:
	void Init() override;
};

class PlayerPawn : public GameObject
{
public:
	void Init() override;
	void Tick(float timeElapsed) override;

	GameObject* target = nullptr;
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
};

class Asteroid : public GameObject
{
public:
	void Init() override;
};

template<typename T>
T* Spawn(GameApp* app)
{
	app->gameObjects.push_back(std::make_unique<T>());
	auto obj = static_cast<T*>(app->gameObjects.back().get());
	obj->app = app;
	obj->Init();
	return obj;
}
