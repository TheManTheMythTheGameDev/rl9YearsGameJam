#pragma once
#include "raylib.h"
#include "raymath.h"
#include "grid.h"
#include <vector>

class PhysicsObject
{
public:
	PhysicsObject(Vector2 position = Vector2{ 0.0f, 0.0f }, float radius = 5.0f, Vector2 grav = Vector2{ 0.0f, 0.2f });

	void Step();
	void DebugRender();

	Vector2 GetPosition();
	Vector2 GetGridPosition();
	Vector2 GetVelocity();
	float GetRadius();

	// Returns an array of 4 positions, on the farthest extremes of both axes
	// In order of: left, right, up, down
	Vector2* GetGridEdgePositions();

	Vector2 GetGroundTile();

	std::vector<int> collisions;
protected:
	Vector2 pos, vel; // position and velocity
	float r; // radius
	Vector2 gravity;
	Vector2 ground; // Square below object that is filled
};