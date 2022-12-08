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
	resolvingCollision = false;

	lastPos = Vector2{ 0.0f, 0.0f };
	curPos = Vector2{ 0.0f, 0.0f };
	releaseVel = Vector2{ 0.0f, 0.0f };
	swingVel = 0.0f;
	swingLen = 0.0f;
}

void HookSnake::Update(Camera2D cam, float dt)
{
	startPos = pos;
	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < controls[i].size(); k++)
		{
			if (IsKeyDown(controls[i][k]))
			{
				pos.x += 3.0f * (i * 2 - 1) * 60.0f * dt;
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

	if (attached) gravity = Vector2{ 0.0f, 0.0f };
	else gravity = Vector2{ 0.0f, 0.2f };
	Step(); // Step physics
	if (!attached)
	{
		tailPos = GetScreenToWorld2D(GetMousePosition(), cam);
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
			swingLen = Vector2Distance(pos, tailPos);
		}

		if (collisions.size() > 0)
		{
			if (!resolvingCollision)
			{
				swingVel = -swingVel;
				resolvingCollision = true;
			}
		}
		else
		{
			resolvingCollision = false;
		}

		if (angle < 90.0f)
		{
			swingVel += 0.01f * 60.0f * dt;
		}
		else
		{
			swingVel -= 0.01f * 60.0f * dt;
		}

		if (swingLen < length)
		{
			swingLen += 1.0f * 60.0f * dt;
		}
		else if (swingLen > length)
		{
			swingLen = length;
		}

		angle += swingVel;

		pos = Vector2Add(tailPos, Vector2Scale(Vector2{ cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) }, swingLen));
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
	releaseVel = Vector2Scale(Vector2Subtract(curPos, lastPos), 60.0f * dt);

	attachedLastFrame = attached;
	lastPos = curPos;

	posChange = Vector2Subtract(pos, startPos);
}

void HookSnake::Draw()
{
	DrawCircle(pos.x, pos.y, r, GREEN);
	DrawCircle(tailPos.x, tailPos.y, r, GREEN);
	// Vector2 thing = Vector2Add(tailPos, Vector2Scale(Vector2{ cosf(targetAngle * DEG2RAD), sinf(targetAngle * DEG2RAD) }, length));
	// DrawCircle(thing.x, thing.y, r, BLUE);

	Vector2 curveStart = pos;
	Vector2 curveEnd = tailPos;
	Vector2 c1 = Vector2{ 0.0f, 0.0f };
	Vector2 c2 = Vector2{ 0.0f, 0.0f };
	float dx = curveEnd.x - curveStart.x;
	float dy = curveEnd.y - curveStart.y;
	float len = sqrtf(dx * dx + dy * dy);
	float angle = atan2f(dy, dx);
	if (len >= length)
	{
		// curveEnd.x = curveStart.x + 100 * cosf(angle);
		// curveEnd.y = curveStart.y + 100 * sinf(angle);
		c1.x = curveStart.x;
		c1.y = curveStart.y;
		c2.x = curveEnd.x;
		c2.y = curveEnd.y;
	}
	else
	{
		float a = length;
		float b = length * cosf(30.0f * PI / 180.0f);
		float handleDist = sqrtf(b * b * (1.0f - len * len / (a * a)));
		c1.x = curveStart.x + handleDist * sinf(angle);
		c1.y = curveStart.y - handleDist * cosf(angle);
		c2.x = curveEnd.x - handleDist * sinf(angle);
		c2.y = curveEnd.y + handleDist * cosf(angle);
	}
	DrawLineBezierCubic(curveStart, curveEnd, c1, c2, r * 2.0f, GREEN);
	
	// Draw eyes
	Vector2 posChangeNorm = Vector2Normalize(posChange);
	float ang = atan2f(posChangeNorm.y, posChangeNorm.x) * RAD2DEG;
	float eyeWidth = 4.0f;
	float eyeHeight = 2.0f;
	Vector2 eyeCenter = Vector2{ pos.x, pos.y };
	Rectangle rect1 = Rectangle{ eyeCenter.x, eyeCenter.y, eyeWidth, eyeHeight };

	Rectangle rect2 = Rectangle{ eyeCenter.x, eyeCenter.y, eyeWidth, eyeHeight };

	DrawRectanglePro(rect1, Vector2{ (eyeWidth / 2.0f), (r / 2.0f) + (eyeHeight / 2.0f) }, ang, BLACK);
	DrawRectanglePro(rect2, Vector2{ (eyeWidth / 2.0f), -(r / 2.0f) + (eyeHeight / 2.0f) }, ang, BLACK);
}