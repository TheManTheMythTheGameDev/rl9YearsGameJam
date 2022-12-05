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
}

void Snake::Update()
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

	Step(); // step physics object

	if (Vector2Equals(pos, nodes[0]) != 1)
	{
		nodes.pop_back(); // Remove last node
		nodes.insert(nodes.begin(), Vector2{ pos.x, pos.y });
		curRotNode = 1;
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
					if (curRotNode == 1)
					{
						if (angle <= -90)
						{
							turnDir = false;
						}
						else
						{
							turnDir = true;
						}
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
}

void Snake::Draw()
{
	DrawCircle(pos.x, pos.y, r * 1.3f, GREEN);
	for (int i = 0; i < nodes.size(); i++)
	{
		DrawCircle(nodes[i].x, nodes[i].y, r, GREEN);
	}
}