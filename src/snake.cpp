#include "snake.h"

enum ControlsIndices
{
	LEFT = 0,
	RIGHT,
	UP
};

Snake::Snake(Vector2 position, float spacing)
{
	pos = position;
	r = 5.0f;

	vel = Vector2{ 0.0f, 0.0f };

	controls[LEFT] = { KEY_LEFT, KEY_A };
	controls[RIGHT] = { KEY_RIGHT, KEY_D };
	controls[UP] = { KEY_UP, KEY_W, KEY_Z, KEY_SPACE };

	nodeSpacing = spacing;

	for (int i = 0; i < 7; i++)
	{
		nodes.push_back(Vector2{ pos.x, pos.y - (i * nodeSpacing) });
	}

	curRotNode = 1;
	decidedDir = false;

	lastPos = Vector2{ 0.0f, 0.0f };
	lastDirLeft = false;

	lastCheckpoint = GetGridPosition();
	checkpointBeforeLast = lastCheckpoint;
}

bool Snake::Update(float dt)
{
	lastPos = pos;

	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < controls[i].size(); k++)
		{
			if (IsKeyDown(controls[i][k]))
			{
				pos.x += 3.0f * (i * 2 - 1) * 60.0f * dt;
				lastDirLeft = (i == 0);
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

	Step(); // step physics object

	if (Vector2Equals(pos, nodes[0]) != 1)
	{
		nodes.pop_back(); // Remove last node
		nodes.insert(nodes.begin(), Vector2{ pos.x, pos.y });
		curRotNode = 1;
		decidedDir = false;
	}
	else
	{
		// Have tail drop down & interact with environment
		if (curRotNode < nodes.size())
		{
			// Create dummy physics object (only for collision detection, no gravity)
			PhysicsObject temp = PhysicsObject(nodes[curRotNode], r, Vector2{ 0.0f, 0.0f });
			temp.Step();
			bool colliding = (temp.collisions.size() > 0);

			if (!colliding)
			{
				Vector2 diff = Vector2Normalize(Vector2Subtract(nodes[curRotNode], nodes[curRotNode - 1]));
				float angle = atan2f(diff.y, diff.x) * RAD2DEG;
				// if (curRotNode == 1) printf("new landing\n");
				// printf("diff x: %f, diff y: %f, ang: %f\n", diff.x, diff.y, angle);
				for (int i = 0; i < 20; i++)
				{
					if (!decidedDir)
					{
						if (angle < -90)
						{
							turnDir = false;
						}
						else if (FloatEquals(angle, -90) == 1)
						{
							turnDir = lastDirLeft;
						}
						else
						{
							turnDir = true;
						}
						decidedDir = true;
					}
					if (turnDir)
					{
						angle += 5.0f;
					}
					else
					{
						angle -= 5.0f;
					}
					Vector2 newPos = Vector2{ cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) };
					nodes[curRotNode] = Vector2Add(nodes[curRotNode - 1], Vector2Scale(newPos, nodeSpacing));
					if (fabsf(angle - (90.0f)) < 10.0f)
					{
						curRotNode++;
						break;
					}
					temp = PhysicsObject(nodes[curRotNode], r, Vector2{ 0.0f, 0.0f });
					temp.Step();
					bool colliding = (temp.collisions.size() > 0);
					if (colliding)
					{
						curRotNode++;
						break;
					}
				}
			}
			else
			{
				curRotNode++;
			}
		}
		else
		{
			decidedDir = false;
		}
	}

	// Adjust all nodes to maintain same distance between first and second node
	// Start for loop at second node, first node doesn't need to be changed
	for (int i = 1; i < nodes.size(); i++)
	{
		Vector2 diff = Vector2Scale(Vector2Normalize(Vector2Subtract(nodes[i], nodes[i - 1])), nodeSpacing);
		/*if (Vector2Equals(nodes[i], nodes[i - 1]) == 1)
		{
			diff = Vector2{ 0.0f, -nodeSpacing };
		}*/
		nodes[i] = Vector2Add(nodes[i - 1], diff);
	}

	posChange = Vector2Subtract(pos, lastPos);

	// Detect if on checkpoint
	if (GetGridAt(GetGridPosition()) == 4)
	{
		lastCheckpoint = GetGridPosition();
	}
	
	if (Vector2Equals(lastCheckpoint, checkpointBeforeLast) != 1)
	{
		LaunchParticles(Vector2Multiply(lastCheckpoint, Vector2{ GRID_W, GRID_H }));
	}

	checkpointBeforeLast = lastCheckpoint;

	return DetectAppleCollisions();
}

void Snake::DrawThickLine(Vector2 start, Vector2 end, float diameter, Color col)
{
	float dist = nodeSpacing; // Vector2Distance(start, end);
	float numCircles = dist / 2.0f;
	for (int i = 0; i < numCircles; i++)
	{
		DrawCircle(Lerp(start.x, end.x, (float)i / (float)numCircles), Lerp(start.y, end.y, (float)i / (float)numCircles), diameter / 2.0f, col);
	}
}

bool Snake::DetectAppleCollisions()
{
	for (int i = 0; i < allApples.size(); i++)
	{
		for (int s = 0; s < nodes.size(); s++)
		{
			if (CheckCollisionCircles(allApples[i]->GetPosition(), allApples[i]->GetRadius(), nodes[s], r))
			{
				return true;
			}
		}
	}
	return false;
}

void Snake::Draw()
{
	DrawCircle(nodes[0].x, nodes[0].y, r, GREEN);
	for (int i = 1; i < nodes.size(); i++)
	{
		// DrawCircle(nodes[i].x, nodes[i].y, r, GREEN);
		DrawThickLine(nodes[i - 1], nodes[i], r * 2.0f, GREEN);
	}

	// Draw eyes
	Vector2 posChangeNorm = Vector2Normalize(posChange);
	float ang = atan2f(posChangeNorm.y, posChangeNorm.x) * RAD2DEG;
	float eyeWidth = 4.0f;
	float eyeHeight = 2.0f;
	Vector2 eyeCenter = Vector2{ nodes[0].x, nodes[0].y};
	Rectangle rect1 = Rectangle{ eyeCenter.x, eyeCenter.y, eyeWidth, eyeHeight };
	
	Rectangle rect2 = Rectangle{ eyeCenter.x, eyeCenter.y, eyeWidth, eyeHeight };

	DrawRectanglePro(rect1, Vector2{ (eyeWidth / 2.0f), (r / 2.0f) + (eyeHeight / 2.0f) }, ang, BLACK);
	DrawRectanglePro(rect2, Vector2{ (eyeWidth / 2.0f), -(r / 2.0f) + (eyeHeight / 2.0f) }, ang, BLACK);
}

Vector2 Snake::GetLastCheckpoint()
{
	return lastCheckpoint;
}
