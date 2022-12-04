#pragma once
#include "raylib.h"

// Size of grid along x and y axes
#define GRID_X 16
#define GRID_Y 16

// Width and height of one square on the grid
#define GRID_W 16
#define GRID_H 16

extern unsigned int grid[GRID_X][GRID_Y];

void InitGrid();

void DrawGrid();

unsigned int GetGridAt(Vector2 at);