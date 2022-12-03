#include "normal_snake.h"

Vector2 up = Vector2{ 0.0f, -1.0f };
Vector2 down = Vector2{ 0.0f, 1.0f };
Vector2 left = Vector2{ -1.0f, 0.0f };
Vector2 right = Vector2{ 1.0f, 0.0f };

enum ControlsIndices
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

Vector2 directions[4] = { up, down, left, right };

NormalSnake::NormalSnake(Vector2 startPos, Vector2 dir, float movementTime)
{
	controls[UP] = KEY_UP;
	controls[DOWN] = KEY_DOWN;
	controls[LEFT] = KEY_LEFT;
	controls[RIGHT] = KEY_RIGHT;

	ballSize = 5.0f;

	balls = std::vector<Ball>{ Ball{ startPos, dir } };

	direction = dir;

	moveTimer = 0.0f;
	moveStep = movementTime;
}

void NormalSnake::Step(float dt)
{
	moveTimer += dt;
	if (moveTimer >= moveStep)
	{
		Vector2 curPos = balls[balls.size() - 1].position; // Get current head position
		balls.erase(balls.begin()); // Remove last ball
		curPos = Vector2Add(curPos, Vector2Scale(direction, 2.0f));
		balls.push_back(Ball{ curPos, direction }); // Move head

		moveTimer -= moveStep;
	}

	for (int i = 0; i < 4; i++)
	{
		if (IsKeyPressed(controls[i]))
		{
			// Player can't turn in opposite direction
			if (direction.x != -directions[i].x && direction.y != -directions[i].y)
			{
				direction = directions[i];
			}
		}
	}
}

void NormalSnake::Draw()
{
	for (int i = 0; i < balls.size(); i++)
	{
		DrawCircle(balls[i].position.x * ballSize, balls[i].position.y * ballSize, ballSize, GREEN);
	}
}