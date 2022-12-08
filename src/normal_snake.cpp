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

void DrawThickLine(Vector2 start, Vector2 end, float diameter, Color col)
{
	float dist = BALL_SIZE; // Vector2Distance(start, end);
	float numCircles = dist / 2.0f;
	for (int i = 0; i < numCircles; i++)
	{
		DrawCircle(Lerp(start.x, end.x, (float)i / (float)numCircles), Lerp(start.y, end.y, (float)i / (float)numCircles), diameter / 2.0f, col);
	}
}

NormalSnake::NormalSnake(Vector2 startPos, Vector2 dir, float movementTime)
{
	controls[UP] = KEY_UP;
	controls[DOWN] = KEY_DOWN;
	controls[LEFT] = KEY_LEFT;
	controls[RIGHT] = KEY_RIGHT;

	balls = std::vector<Ball>{ Ball{ startPos, dir } };
	ballCounter = 1;

	direction = dir;
	targetDir = direction;

	moveTimer = 0.0f;
	moveStep = movementTime;

	beingEaten = false;
	eatenID = 0;

	ateTimer = 0.0f;
	ateAnimationTime = 1.0f;
	ateStartSize = 0.0f;
}

SnakeState NormalSnake::Step(float dt)
{
	if (balls.size() > 0)
	{
		moveTimer += dt;
		if (moveTimer >= moveStep)
		{
			direction = targetDir;
			Vector2 curPos = balls[balls.size() - 1].position; // Get current head position
			bool deleteBack = true;

			for (int a = 0; a < apples.size(); a++)
			{
				if (apples[a].pos.x == curPos.x && apples[a].pos.y == curPos.y)
				{
					if (ballCounter < 7)
					{
						apples.erase(apples.begin() + a);
						deleteBack = false;
						ballCounter++;
					}
					else
					{
						beingEaten = true;
						eatenID = a;
					}
				}
			}

			if (deleteBack) balls.erase(balls.begin()); // Remove last ball
			else SpawnApple();
			curPos = Vector2Add(curPos, direction);
			if (!beingEaten) balls.push_back(Ball{ curPos, direction }); // Move head
			else
			{
				apples[eatenID].size += 5;
				ateStartSize = apples[eatenID].size;
			}

			moveTimer -= moveStep;
		}

		for (int i = 0; i < 4; i++)
		{
			if (IsKeyPressed(controls[i]))
			{
				// Player can't turn in opposite direction
				if (direction.x != -directions[i].x && direction.y != -directions[i].y)
				{
					targetDir = directions[i];
					break;
				}
			}
		}

		if (beingEaten)
		{
			return EATING;
		}
		else
		{
			return OK;
		}
	}
	ateTimer += dt;
	apples[eatenID].size = EaseElasticInOut(ateTimer, ateStartSize, -ateStartSize, ateAnimationTime);
	if (apples[eatenID].size <= 0)
	{
		return ENDSCENE;
	}
	return ATE;
}

void NormalSnake::Draw()
{
	float movementPercent = moveTimer / moveStep;
	Vector2 headPos = Vector2{ 0.0f, 0.0f };
	for (int i = balls.size() - 1; i >= 0; i--)
	{
		Vector2 curPos = balls[i].position;
		if (i == 0 || i == balls.size() - 1)
		{
			curPos = Vector2Lerp(curPos, Vector2Add(curPos, balls[Clamp(i + 1, 0, balls.size() - 1)].direction), movementPercent);
		}
		if (i == balls.size() - 1)
		{
			DrawCircle(curPos.x * BALL_SIZE, curPos.y * BALL_SIZE, BALL_SIZE / 2.0f, GREEN);
			headPos = curPos;
		}
		else
		{
			Vector2 nextPos = balls[i + 1].position;
			if (i == balls.size() - 2) nextPos = headPos;
			DrawThickLine(Vector2{ curPos.x * BALL_SIZE, curPos.y * BALL_SIZE }, Vector2{ nextPos.x * BALL_SIZE, nextPos.y * BALL_SIZE }, BALL_SIZE, GREEN);
		}
	}

	if (balls.size() > 0)
	{
		// Draw eyes
		float ang = atan2f(direction.y, direction.x) * RAD2DEG;
		float eyeWidth = 4.0f;
		float eyeHeight = 2.0f;
		Vector2 eyeCenter = Vector2{ headPos.x, headPos.y };
		Rectangle rect1 = Rectangle{ eyeCenter.x * BALL_SIZE, eyeCenter.y * BALL_SIZE, eyeWidth, eyeHeight };

		Rectangle rect2 = Rectangle{ eyeCenter.x * BALL_SIZE, eyeCenter.y * BALL_SIZE, eyeWidth, eyeHeight };

		DrawRectanglePro(rect1, Vector2{ (eyeWidth / 2.0f), (BALL_SIZE / 4.0f) + (eyeHeight / 2.0f) }, ang, BLACK);
		DrawRectanglePro(rect2, Vector2{ (eyeWidth / 2.0f), -(BALL_SIZE / 4.0f) + (eyeHeight / 2.0f) }, ang, BLACK);
	}
}