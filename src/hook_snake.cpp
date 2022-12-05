#include "hook_snake.h"

enum ControlsIndices
{
	LEFT = 0,
	RIGHT,
	UP
};

HookSnake::HookSnake(Vector2 position, float len)
{
	tailPos = position;
	pos = position;
	r = 5.0f;

	vel = Vector2{ 0.0f, 0.0f };

	controls[LEFT] = { KEY_LEFT, KEY_A };
	controls[RIGHT] = { KEY_RIGHT, KEY_D };
	controls[UP] = { KEY_UP, KEY_W, KEY_Z, KEY_SPACE };

	length = len;
	attached = false;
	attachedLastFrame = false;
	angle = 0.0f;
	startAngle = 0.0f;
	targetAngle = 0.0f;
	swingTimer = 0.0f;
}

void HookSnake::Update()
{
	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < controls[i].size(); k++)
		{
			if (IsKeyDown(controls[i][k]))
			{
				pos.x += 3.0f * (i * 2 - 1);
			}
		}
	}

	for (int i = 0; i < controls[UP].size(); i++)
	{
		if (IsKeyDown(controls[UP][i]))
		{
			if (std::find(collisions.begin(), collisions.end(), 3) != collisions.end())
			{
				vel.y = -5.0f;
			}
		}
	}
	unsigned int tailGrid = 0;

	float gridX = tailPos.x / GRID_W;
	float gridY = tailPos.y / GRID_H;

	// Truncate the values
	int tX = (int)gridX;
	int tY = (int)gridY;

	tailGrid = GetGridAt(Vector2{ (float)tX, (float)tY });

	if (!attached)
	{
		Step(); // Step physics

		tailPos = GetMousePosition();
		if (Vector2DistanceSqr(tailPos, pos) > length * length)
		{
			Vector2 dir = Vector2Normalize(Vector2Subtract(tailPos, pos));
			tailPos = Vector2Add(pos, Vector2Scale(dir, length));
		}
	}
	else
	{
		tailPos = Vector2{ ((float)tX * GRID_W) + (GRID_W / 2.0f), ((float)tY * GRID_H) + 3.0f };
	}

	if (tailGrid == 2)
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			attached = true;
		}
		else
		{
			attached = false;
		}
	}
	else
	{
		attached = false;
	}

	if (attached)
	{
		swingTimer += GetFrameTime();
		if (!attachedLastFrame)
		{
			Vector2 diff = Vector2Normalize(Vector2Subtract(pos, tailPos));
			angle = atan2f(diff.y, diff.x) * RAD2DEG;
			if (angle < 90.0f)
			{
				targetAngle = 180.0f - angle;
			}
			else
			{
				targetAngle = 90.0f - (angle - 90.0f);
			}
			swingTimer = 0.0f;
			startAngle = angle;
		}
		else
		{
			// targetAngle += targetAngle < 90 ? (-0.1f) : (0.1f);
		}
		float t = PI * sqrtf((Vector2Distance(tailPos, pos) / 56.0f) / 9.8f);
		angle = Lerp(startAngle, targetAngle, swingTimer / t);
		// angle = EaseSineInOut(swingTimer, startAngle, targetAngle, t);

		pos = Vector2Add(tailPos, Vector2Scale(Vector2{ cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) }, length));

		printf("timer: %f, t: %f, ang: %f\n", swingTimer, t, angle);

		if (swingTimer >= t)
		{
			startAngle = angle;
			if (angle < 90.0f)
			{
				targetAngle = 180.0f - angle;
			}
			else
			{
				targetAngle = 90.0f - (angle - 90.0f);
			}
			swingTimer = 0.0f;
		}
	}

	attachedLastFrame = attached;
}

void HookSnake::Draw()
{
	DrawCircle(pos.x, pos.y, r, GREEN);
	DrawCircle(tailPos.x, tailPos.y, r, RED);
	// Vector2 thing = Vector2Add(tailPos, Vector2Scale(Vector2{ cosf(targetAngle * DEG2RAD), sinf(targetAngle * DEG2RAD) }, length));
	// DrawCircle(thing.x, thing.y, r, BLUE);
}