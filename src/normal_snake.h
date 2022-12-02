#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <map>

extern Vector2 up, down, left, right;

class NormalSnake
{
	NormalSnake(Vector2 startPos, Vector2 dir = right);

	void Step(float dt);

	float ballSize;

	std::map<Vector2, KeyboardKey> controls;
private:
	std::vector<Vector2> balls;
	int ballCounter; // Because balls vector is constantly being changed
};