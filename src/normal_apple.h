#pragma once
#include <vector>
#include "raylib.h"

#define BALL_SIZE 10.0f

struct Apple
{
	Vector2 pos;
	float size;
};

extern std::vector<Apple> apples;

void InitApples();

void SpawnApple();

void DrawApples();