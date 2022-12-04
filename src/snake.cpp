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

	nodes.pop_back(); // Remove last node
	nodes.insert(nodes.begin(), Vector2{ pos.x, pos.y });
}

void Snake::Draw()
{
	DrawCircle(pos.x, pos.y, r * 1.3f, GREEN);
	for (int i = 0; i < nodes.size(); i++)
	{
		DrawCircle(nodes[i].x, nodes[i].y, r, GREEN);
	}
}