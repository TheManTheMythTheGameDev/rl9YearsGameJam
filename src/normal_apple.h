#pragma once
#include <vector>
#include "raylib.h"

#define BALL_SIZE 10.0f

extern std::vector<Vector2> apples;

void InitApples();

void SpawnApple();

void DrawApples();