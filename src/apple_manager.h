#pragma once
#include "apple.h"

extern std::vector<BadApple*> allApples;

void InitEnemies();

void UpdateEnemies(PhysicsObject player, float dt);

void DrawEnemies();

void UnloadEnemies();
