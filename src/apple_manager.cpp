#include "apple_manager.h"

std::vector<BadApple*> allApples;

void InitEnemies()
{
	allApples = std::vector<BadApple*>();

	BadApple* startingApple = new BadApple(Vector2{ 16.0f, 100.0f });
	allApples.push_back(startingApple);
}

void UpdateEnemies(PhysicsObject player, float dt)
{
	for (int i = 0; i < allApples.size(); i++)
	{
		BadApple* cur = allApples[i];
		cur->SetTargetPosition(player);
		cur->Update(dt);
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
