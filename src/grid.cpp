#include "grid.h"

// Grid starts out filled with 0s
unsigned int grid[GRID_X][GRID_Y] = {{0}};

void InitGrid()
{
	for (int i = 0; i < GRID_X; i++)
	{
		grid[i][GRID_Y - 1] = 1;
	}

	grid[5][GRID_Y - 5] = 1;
	grid[5][GRID_Y - 4] = 2; // Hook
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
		}
	}
}

unsigned int GetGridAt(Vector2 at)
{
	return grid[(int)at.x][(int)at.y];
}