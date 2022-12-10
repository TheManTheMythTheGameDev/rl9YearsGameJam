#include "apple_manager.h"

std::vector<BadApple*> allApples;

std::vector<EnemyZone*> spawnZones;

const float spawnTime = 1.0f;

void InitEnemies()
{
	allApples = std::vector<BadApple*>();

	// BadApple* startingApple = new BadApple(Vector2{ 16.0f, 100.0f });
	// allApples.push_back(startingApple);
}

void UpdateEnemies(PhysicsObject player, float dt)
{
	for (int i = 0; i < allApples.size(); i++)
	{
		BadApple* cur = allApples[i];
		cur->SetTargetPosition(player);
		cur->Update(dt);
	}

	Vector2 playerGridPos = player.GetGridPosition();
	for (int s = 0; s < spawnZones.size(); s++)
	{
		EnemyZone* curZone = spawnZones[s];
		if (playerGridPos.x >= curZone->start.x && playerGridPos.x <= curZone->end.x && playerGridPos.y >= curZone->start.y && playerGridPos.y <= curZone->end.y)
		{
			curZone->activated = true;
		}

		if (curZone->activated)
		{
			if (curZone->spawned < curZone->enemyCount)
			{
				curZone->spawnTimer += dt;
				if (curZone->spawnTimer >= spawnTime)
				{
					curZone->spawnTimer -= spawnTime;

					BadApple* curApple = new BadApple(Vector2Multiply(curZone->spawnPos, Vector2{ GRID_W, GRID_H }));
					allApples.push_back(curApple);

					curZone->spawned++;
				}
			}
		}
	}
}

void DrawEnemies()
{
	for (int i = 0; i < allApples.size(); i++)
	{
		allApples[i]->Draw();
	}
}

void UnloadEnemies()
{
	for (int i = allApples.size() - 1; i >= 0; i--)
	{
		delete allApples[i];
		allApples.erase(allApples.begin() + i);
	}
}

void InitSpawnZones()
{
	spawnZones = std::vector<EnemyZone*>();
}

void AddSpawnZone(EnemyZone* target)
{
	spawnZones.push_back(target);
}

void UnloadSpawnZones()
{
	for (int i = spawnZones.size() - 1; i >= 0; i--)
	{
		delete spawnZones[i];
		spawnZones.erase(spawnZones.begin() + i);
	}
}