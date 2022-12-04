#include "grid.h"

// Grid starts out filled with 0s
unsigned int grid[GRID_X][GRID_Y] = {{0}};

void InitGrid()
{
	for (int i = 0; i < GRID_X; i++)
	{
		grid[i][GRID_Y - 1] = 1;
	}

	grid[10][GRID_Y - 4] = 1;
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
		}
	}
}

unsigned int GetGridAt(Vector2 at)
{
	return grid[(int)at.x][(int)at.y];
}