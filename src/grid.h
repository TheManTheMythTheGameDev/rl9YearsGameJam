#pragma once
#include "raylib.h"
#include "raymath.h"

// Size of grid along x and y axes
#define GRID_X 100
#define GRID_Y 80

// Width and height of one square on the grid
#define GRID_W 16
#define GRID_H 16

extern unsigned int grid[GRID_X][GRID_Y];

void InitGrid();

void DrawGrid();

unsigned int GetGridAt(Vector2 at);