#include "normal_apple.h"

std::vector<Vector2> apples;

void InitApples()
{
	apples.push_back({ Vector2{ 10.0f, 0.0f } });
}

void SpawnApple()
{
	apples.push_back({ Vector2{ (float)GetRandomValue(-10, 10), (float)GetRandomValue(-5, 5) } });
}

void DrawApples()
{
	for (int i = 0; i < apples.size(); i++)
	{
		DrawCircle(apples[i].x * BALL_SIZE, apples[i].y * BALL_SIZE, BALL_SIZE / 2.0f, RED);
	}
}