#pragma once
#include "raylib.h"
#include "physics_object.h"

class HookSnake : public PhysicsObject
{
public:
	HookSnake(Vector2 position = Vector2{ 0.0f, 0.0f });

	void Update();
	void Draw();

	std::vector<int> controls[3];
};