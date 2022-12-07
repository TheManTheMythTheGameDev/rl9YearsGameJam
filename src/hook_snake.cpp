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

	lastPos = Vector2{ 0.0f, 0.0f };
	curPos = Vector2{ 0.0f, 0.0f };
	releaseVel = Vector2{ 0.0f, 0.0f };
	swingVel = 0.0f;
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
				vel.x = 0.0f;
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

	if (tailGrid != 2)
	{
		for (int x = -1; x < 2; x++)
		{
			for (int y = -1; y < 2; y++)
			{
				Vector2 testPoint = Vector2{ (float)(tX + x), (float)(tY + y) };
				if (GetGridAt(testPoint) == 2)
				{
					tX = (int)testPoint.x;
					tY = (int)testPoint.y;
					tailGrid = 2;
				}
			}
		}
	}

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
		if ((IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) && tY <= (int)(pos.y / GRID_H))
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
		if (!attachedLastFrame)
		{
			float velLen = Vector2Length(releaseVel) * 1.2f;
			if (releaseVel.x > 0)
			{
				swingVel -= velLen;
			}
			else
			{
				swingVel += velLen;
			}
			Vector2 relativePos = Vector2Subtract(pos, tailPos);
			relativePos = Vector2Normalize(relativePos);
			angle = atan2f(relativePos.y, relativePos.x) * RAD2DEG;
		}

		if (angle < 90.0f)
		{
			swingVel += 0.01f;
		}
		else
		{
			swingVel -= 0.01f;
		}

		angle += swingVel;

		pos = Vector2Add(tailPos, Vector2Scale(Vector2{ cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) }, length));
	}
	else
	{
		if (attachedLastFrame)
		{
			vel = Vector2Add(vel, releaseVel);
		}
		swingVel = 0.0f;
	}

	curPos = pos;
	releaseVel = Vector2Subtract(curPos, lastPos);

	attachedLastFrame = attached;
	lastPos = curPos;
}

void HookSnake::Draw()
{
	DrawCircle(pos.x, pos.y, r, GREEN);
	DrawCircle(tailPos.x, tailPos.y, r, RED);
	// Vector2 thing = Vector2Add(tailPos, Vector2Scale(Vector2{ cosf(targetAngle * DEG2RAD), sinf(targetAngle * DEG2RAD) }, length));
	// DrawCircle(thing.x, thing.y, r, BLUE);
}