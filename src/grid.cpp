#include "grid.h"

// Grid starts out filled with 0s
unsigned int grid[GRID_X][GRID_Y] = {{0}};

void InitGrid()
{
	for (int i = 0; i < GRID_X; i++)
	{
		grid[i][GRID_Y - 1] = 1;
	}

	grid[13][GRID_Y - 2] = 4; // Checkpoint

	for (int i = 0; i < 5; i++)
	{
		grid[8 + i][GRID_Y - 3] = 1;
		grid[13 + i][GRID_Y - 10] = 1;
	}
	grid[10][GRID_Y - 5] = 3; // Orb for tail control
	grid[18][GRID_Y - 9] = 2; // Hook
}

void DrawGrid()
{
	for (int x = 0; x < GRID_X; x++)
	{
		for (int y = 0; y < GRID_Y; y++)
		{
			unsigned int curSquare = grid[x][y];
			if (curSquare == 1)
			{
				DrawRectangle(x * GRID_W, y * GRID_H, GRID_W, GRID_H, BLACK);
			}
			else if (curSquare == 2)
			{
				float hookHalfWidth = 3.0f;
				DrawRectangle((x * GRID_W) + (GRID_W / 2.0f) - hookHalfWidth, (y * GRID_H), hookHalfWidth * 2.0f, 7.0f, GRAY);
			}
			else if (curSquare == 3)
			{
				float orbRadius = 5.0f;
				DrawCircle((x * GRID_W) + (GRID_W / 2.0f), (y * GRID_H) + GRID_H - orbRadius - 1.0f, orbRadius, GOLD);
			}
			else if (curSquare == 4)
			{
				// Draw a flag
				// -----------
				// Draw pole
				float poleWidth = 3;
				float poleHeight = 14;
				DrawRectangle((x * GRID_W) + (GRID_W / 2.0f) - (poleWidth / 2.0f), (y * GRID_H) + GRID_H - poleHeight, poleWidth, poleHeight, LIGHTGRAY);
				// Draw flag
				Vector2 v1 = Vector2{ (x * GRID_W) + (GRID_W / 2.0f) + (poleWidth / 2.0f), (y * GRID_H) + GRID_H - poleHeight };
				Vector2 v2 = Vector2Add(v1, Vector2{ 8.0f, 4.0f });
				Vector2 v3 = Vector2Add(v1, Vector2{ 0.0f, 8.0f });
				DrawTriangle(v3, v2, v1, GREEN);
			}
		}
	}
}

unsigned int GetGridAt(Vector2 at)
{
	return grid[(int)at.x][(int)at.y];
}