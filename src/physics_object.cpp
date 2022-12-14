#include "physics_object.h"

Vector2 oppDirs[] = {
	Vector2{ 1.0f, 0.0f },
	Vector2{ -1.0f, 0.0f },
	Vector2{ 0.0f, 1.0f },
	Vector2{ 0.0f, -1.0f }
};

PhysicsObject::PhysicsObject(Vector2 position, float radius, Vector2 grav)
{
	pos = position;
	r = radius;

	vel = Vector2{ 0.0f, 0.0f };
	gravity = grav;

	ground = Vector2{ -1.0f, -1.0f };
}

void PhysicsObject::Step()
{
	float dt = GetFrameTime();
	vel = Vector2Add(vel, gravity);

	vel = Vector2Scale(vel, 0.99f);

	pos = Vector2Add(pos, Vector2Scale(vel, 60.0f * dt));
	Vector2* squares = GetGridEdgePositions();

	collisions.clear();
	bool correctedUp = false;

	for (int i = 0; i < 4; i++)
	{
		unsigned int curSquare = GetGridAt(squares[i]);
		if (curSquare == 1)
		{
			if (i == 3 && correctedUp) break;
			collisions.push_back(i);

			Vector2 targetPos = Vector2Add(squares[i], oppDirs[i]);
			targetPos.x *= GRID_W;
			targetPos.y *= GRID_H;
			bool moveX = false;
			bool oppPos = false;
			if (oppDirs[i].x == 0.0f)
			{
				targetPos.x = pos.x;
				if (oppDirs[i].y > 0.0f)
				{
					oppPos = true;
				}
			}
			else
			{
				targetPos.y = pos.y;
				moveX = true;
				if (oppDirs[i].x > 0.0f)
				{
					oppPos = true;
				}
			}
			targetPos = Vector2Add(targetPos, Vector2Scale(oppDirs[i], ((oppPos) ? 0.0f : - 1.0f) * (moveX ? GRID_W : GRID_H)));
			targetPos = Vector2Add(targetPos, Vector2Scale(oppDirs[i], r));

			pos = targetPos;

			(moveX ? vel.x : vel.y) = 0.0f;

			if (i == 2) correctedUp = true;
		}
	}

	int tX = (int)(pos.x / GRID_W);
	int tY = (int)(pos.y / GRID_H);
	Vector2 curGrid = Vector2{ (float)tX, (float)tY };
	int i = 0;
	bool shouldStop = false;
	while (!shouldStop)
	{
		if (curGrid.y + i > GRID_Y)
		{
			shouldStop = true;
			ground = Vector2{ -1.0f, -1.0f };
			break;
		}

		if (GetGridAt(Vector2{ curGrid.x, curGrid.y + (float)i }) == 1)
		{
			shouldStop = true;
			ground = Vector2{ curGrid.x, curGrid.y + (float)i };
			break;
		}

		i++;
	}
}

void PhysicsObject::DebugRender()
{
	DrawCircle(pos.x, pos.y, r, BLUE);
}

Vector2 PhysicsObject::GetPosition()
{
	return pos;
}

Vector2 PhysicsObject::GetGridPosition()
{
	// Truncate the floats
	int x = (int)(pos.x / GRID_W);
	int y = (int)(pos.y / GRID_H);

	return Vector2{ (float)x, (float)y };
}

Vector2 PhysicsObject::GetVelocity()
{
	return vel;
}

float PhysicsObject::GetRadius()
{
	return r;
}

Vector2* PhysicsObject::GetGridEdgePositions()
{
	Vector2 result[4] = { 0 };
	int i = 0;
	for (int x = -1; x < 2; x += 2)
	{
		float xPos = x;
		xPos *= r;
		xPos += pos.x;

		float yPos = pos.y;

		// Truncate x and y
		int tX = (int)(xPos / GRID_W);
		int tY = (int)(yPos / GRID_H);

		result[i] = Vector2{ (float)tX, (float)tY };
		i++;
	}

	for (int y = -1; y < 2; y += 2)
	{
		float xPos = pos.x;

		float yPos = y;
		yPos *= r;
		yPos += pos.y;

		// Truncate x and y
		int tX = (int)(xPos / GRID_W);
		int tY = (int)(yPos / GRID_H);

		result[i] = Vector2{ (float)tX, (float)tY };
		i++;
	}

	return result;
}

Vector2 PhysicsObject::GetGroundTile()
{
	return ground;
}
