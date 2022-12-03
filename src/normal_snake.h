#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <map>

extern Vector2 up, down, left, right;

struct Ball
{
	Vector2 position;
	Vector2 direction;
};

class NormalSnake
{
public:
	NormalSnake(Vector2 startPos = Vector2{ 0.0f, 0.0f }, Vector2 dir = right, float movementTime = 0.5f);

	void Step(float dt);
	void Draw();

	float ballSize;

	int controls[4];
private:
	std::vector<Ball> balls;
	int ballCounter; // Because balls vector is constantly being changed
	Vector2 direction;
	float moveTimer;
	float moveStep;
};