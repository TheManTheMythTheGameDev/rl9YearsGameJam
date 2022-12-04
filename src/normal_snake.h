#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <map>

#include "normal_apple.h"

#include "reasings.h"

extern Vector2 up, down, left, right;

struct Ball
{
	Vector2 position;
	Vector2 direction;
};

enum SnakeState
{
	OK = 0,
	EATING,
	ATE,
	ENDSCENE
};

class NormalSnake
{
public:
	NormalSnake(Vector2 startPos = Vector2{ 0.0f, 0.0f }, Vector2 dir = right, float movementTime = 0.2f);

	SnakeState Step(float dt);
	void Draw();

	int controls[4];
private:
	std::vector<Ball> balls;
	int ballCounter; // Because balls vector is constantly being changed
	Vector2 direction;
	Vector2 targetDir;
	float moveTimer;
	float moveStep;
	bool beingEaten;
	int eatenID;
	float ateTimer;
	float ateAnimationTime;
	float ateStartSize;
};