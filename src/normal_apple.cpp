#include "normal_apple.h"

std::vector<Apple> apples;

void InitApples()
{
	apples.push_back({ Apple{ Vector2{ 10.0f, 0.0f }, BALL_SIZE } });
}

void SpawnApple()
{
	apples.push_back({ Apple{ Vector2{ (float)GetRandomValue(-10, 10), (float)GetRandomValue(-5, 5) }, BALL_SIZE } });
}

void DrawApples()
{
	for (int i = 0; i < apples.size(); i++)
	{
		DrawCircle(apples[i].pos.x * BALL_SIZE, apples[i].pos.y * BALL_SIZE, apples[i].size / 2.0f, RED);
	}
}