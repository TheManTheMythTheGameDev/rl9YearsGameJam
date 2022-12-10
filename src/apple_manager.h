#pragma once
#include "apple.h"

extern std::vector<BadApple*> allApples;

struct EnemyZone
{
	EnemyZone();
	Vector2 start, end;
	int enemyCount;
	Vector2 spawnPos;

	int spawned = 0;
	float spawnTimer = 0.0f;
	bool activated = false;
};

extern std::vector<EnemyZone*> spawnZones;

void InitEnemies();

void UpdateEnemies(PhysicsObject player, float dt);

void DrawEnemies();

void UnloadEnemies();

// Spawn zone functions
void InitSpawnZones();

void AddSpawnZone(EnemyZone* target);

void UnloadSpawnZones();
