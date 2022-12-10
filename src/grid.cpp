#include "grid.h"

// Grid starts out filled with 0s
unsigned int grid[GRID_X][GRID_Y] = {{0}};

void AddBlock(int x, int upFromGround)
{
	grid[x][GRID_Y - 1 - upFromGround] = 1;
}

void AddCheckPoint(int x, int upFromGround)
{
	grid[x][GRID_Y - 1 - upFromGround] = 4;
}

void AddHook(int x, int upFromGround)
{
	grid[x][GRID_Y - 1 - upFromGround] = 2;
}

void AddRect(int startX, int startUp, int endX, int endUp)
{
	for (int x = startX; x < endX + 1; x++)
	{
		for (int y = startUp; y < endUp + 1; y++)
		{
			AddBlock(x, y);
		}
	}
}

void InitGrid()
{
	for (int i = 0; i < GRID_X; i++)
	{
		grid[i][GRID_Y - 1] = 1;
	}

	// Testing area
	/*grid[13][GRID_Y - 2] = 4; // Checkpoint

	for (int i = 0; i < 5; i++)
	{
		grid[8 + i][GRID_Y - 3] = 1;
		grid[13 + i][GRID_Y - 10] = 1;
	}
	grid[10][GRID_Y - 5] = 3; // Orb for tail control
	grid[18][GRID_Y - 9] = 2; // Hook*/

	// Actual game
	AddBlock(14, 1);
	AddBlock(14, 2);
	AddBlock(14, 3);
	
	AddBlock(15, 1);
	AddBlock(15, 2);
	AddBlock(15, 3);
	AddBlock(15, 4);
	AddBlock(15, 5);
	AddBlock(15, 6);

	AddBlock(19, 8);
	AddBlock(20, 8);
	AddBlock(21, 8);

	AddRect(26, 1, 36, 7);
	AddCheckPoint(29, 8);

	AddBlock(37, 8);
	AddBlock(38, 8);
	AddBlock(39, 8);

	AddBlock(40, 5);
	AddBlock(41, 5);
	AddBlock(42, 5);
	AddBlock(43, 5);
	AddBlock(44, 5);
	AddBlock(45, 5);

	AddBlock(46, 8);
	AddBlock(47, 8);
	AddBlock(48, 8);

	AddRect(49, 1, 52, 9);
	AddCheckPoint(50, 10);

	AddRect(53, 12, 56, 12);
	AddRect(49, 15, 52, 15);
	AddRect(53, 18, 56, 18);

	AddRect(30, 16, 48, 19);
	AddCheckPoint(45, 20);

	grid[43][GRID_Y - 21] = 3;

	AddRect(35, 25, 39, 25);
	AddHook(35, 24);
	AddCheckPoint(37, 26);

	AddRect(32, 28, 34, 28);
	AddRect(28, 35, 33, 35);
	AddHook(28, 34);

	AddRect(17, 36, 22, 36);
	AddCheckPoint(19, 37);

	AddRect(0, 39, 3, 39);
	AddRect(3, 42, 17, 42);
	AddHook(7, 41);
	AddHook(12, 41);
	AddCheckPoint(10, 43);

	AddRect(4, 45, 7, 45);
	AddRect(0, 48, 3, 48);
	AddRect(4, 51, 7, 51);

	AddRect(8, 58, 12, 58);
	AddHook(8, 57);
	AddCheckPoint(10, 59);

	int ceilY = 65;
	AddRect(13, ceilY, 80, ceilY + 5);
	for (int x = 13; x < 80; x += 6)
	{
		AddHook(x, ceilY - 1);
	}
	AddRect(81, 58, 100, 58);
	AddCheckPoint(90, 59);
	grid[99][GRID_Y - 60] = 5; // End of game
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
			else if (curSquare == 5)
			{
				float orbRadius = 5.0f;
				DrawCircle((x * GRID_W) + (GRID_W / 2.0f), (y * GRID_H) + GRID_H - orbRadius - 1.0f, orbRadius, DARKGREEN);
			}
		}
	}
}

unsigned int GetGridAt(Vector2 at)
{
	return grid[(int)at.x][(int)at.y];
}