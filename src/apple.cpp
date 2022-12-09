#include "apple.h"

BadApple::BadApple(Vector2 position, Vector2 targetPosition)
{
	pos = position;
	r = 5.0f;

	speed = 1.0f; // Slower than snake
	jumping = false;

	vel = Vector2{ 0.0f, 0.0f };

	targetPos = targetPosition;
	pathfindTarget = Vector2{ -1.0f, -1.0f };
	secondaryPathfindTarget = Vector2{ -1.0f, -1.0f };
}

void BadApple::Update(float dt)
{
	// Run snake AI
	int tX = (int)(pos.x / GRID_W);
	int tY = (int)(pos.y / GRID_H);
	Vector2 appleGrid = Vector2{ (float)tX, (float)tY };

	if (targetPos.GetGroundTile().x != -1.0f && !jumping)
	{
		// Jump up
		if (targetPos.GetGroundTile().y < GetGroundTile().y)
		{
			Vector2 closestGrid = Vector2{ -1.0f, -1.0f };
			float closestDist = std::numeric_limits<float>().max();

			for (int i = 0; i < GRID_X; i++)
			{
				for (int y = 1; y < 3; y++)
				{
					Vector2 curGrid = Vector2{ (float)i, (float)((int)(pos.y / GRID_H) - y) };
					if (GetGridAt(curGrid) == 1 && (GetGridAt(Vector2Add(curGrid, Vector2{ 1.0f, 0.0f })) != 1 || GetGridAt(Vector2Subtract(curGrid, Vector2{ 1.0f, 0.0f })) != 1))
					{
						float curDist = Vector2DistanceSqr(appleGrid, curGrid);
						if (curDist < closestDist)
						{
							closestGrid = curGrid;
							closestDist = curDist;
						}
					}
				}
			}

			secondaryPathfindTarget = closestGrid;

			if (GetGridAt(Vector2Add(closestGrid, Vector2{ 1.0f, 0.0f })) != 1)
			{
				pathfindTarget = Vector2{ closestGrid.x + 2.0f, appleGrid.y };
			}
			else
			{
				pathfindTarget = Vector2{ closestGrid.x - 1.0f, appleGrid.y };
			}
		}
		// Drop down
		else if (targetPos.GetGroundTile().y > GetGroundTile().y || (targetPos.GetPosition().y > pos.y))
		{
			Vector2 closestGrid = Vector2{ -1.0f, -1.0f };
			float closestDist = std::numeric_limits<float>().max();

			bool scanRight = (targetPos.GetPosition().x > pos.x);

			for (int i = appleGrid.x; (scanRight ? i < GRID_X : i > 0); (scanRight ? i++ : i--))
			{
				Vector2 curGrid = Vector2{ (float)i, ground.y };
				if (GetGridAt(curGrid) != 1)
				{
					float curDist = Vector2DistanceSqr(appleGrid, curGrid);
					if (curDist < closestDist)
					{
						closestGrid = curGrid;
						closestDist = curDist;
					}
				}
			}

			pathfindTarget = closestGrid;

			if (Vector2Equals(pathfindTarget, Vector2{ -1.0f, -1.0f }))
			{
				pathfindTarget = targetPos.GetGridPosition();
			}

			if (GetGridAt(Vector2{pathfindTarget.x + 1, pathfindTarget.y}) == 1)
			{
				pathfindTarget.x--;
			}
			else
			{
				pathfindTarget.x++;
			}
			// pathfindTarget.x += 3;
		}
		// On same y level
		else
		{
			pathfindTarget = targetPos.GetPosition();
			pathfindTarget.x /= GRID_W;
			pathfindTarget.y /= GRID_H;
		}
	}
	else
	{
		pathfindTarget = targetPos.GetPosition();
		pathfindTarget.x /= GRID_W;
		pathfindTarget.y /= GRID_H;
	}

	if (pathfindTarget.x * GRID_W > pos.x)
	{
		MoveRight(dt);
	}
	else
	{
		MoveLeft(dt);
	}

	if (Vector2Equals(appleGrid, pathfindTarget) == 1)
	{
		if (Vector2Equals(secondaryPathfindTarget, Vector2{ -1.0f, -1.0f }) != 1)
		{
			pathfindTarget = secondaryPathfindTarget;
			secondaryPathfindTarget = Vector2{ -1.0f, -1.0f };
		}
	}

	if (!FloatEquals(pathfindTarget.x, -1.0f) && !FloatEquals(pathfindTarget.y, -1.0f))
	{
		if (pathfindTarget.y < appleGrid.y)
		{
			if (fabsf((pathfindTarget.x * GRID_W) + (GRID_W / 2.0f) - pos.x) < GRID_W * 2.5f)
			{
				Jump();
				jumping = true;
			}
		}
	}

	if (std::find(collisions.begin(), collisions.end(), 3) != collisions.end())
	{
		jumping = false;
	}

	Step(); // Simulate physics
}

void BadApple::Draw()
{
	DrawCircle(pos.x, pos.y, r, RED);
}

void BadApple::SetTargetPosition(PhysicsObject newPosition)
{
	targetPos = newPosition;
}

void BadApple::Jump()
{
	if (std::find(collisions.begin(), collisions.end(), 3) != collisions.end())
	{
		vel.y -= 5.0f; // Can jump 3 units high - onto square 2 units above it
	}
}

void BadApple::MoveLeft(float dt)
{
	pos.x -= speed * 60.0f * dt;
}

void BadApple::MoveRight(float dt)
{
	pos.x += speed * 60.0f * dt;
}
