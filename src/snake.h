#pragma once
#include "raylib.h"
#include "grid.h"
#include "physics_object.h"
#include <vector>
#include <algorithm>
#include "apple_manager.h"

class Snake : public PhysicsObject
{
public:
	Snake(Vector2 position = Vector2{ 0.0f, 0.0f }, float spacing = 8.0f);

	// Returns whether snake is dead
	bool Update(float dt);
	void Draw();

	Vector2 GetLastCheckpoint();

	std::vector<int> controls[3];

	// index 0 = head
	std::vector<Vector2> nodes;
private:
	float nodeSpacing;
	int curRotNode;
	bool turnDir;
	bool decidedDir;

	Vector2 lastPos;
	Vector2 posChange;
	bool lastDirLeft;

	Vector2 lastCheckpoint;

	void DrawThickLine(Vector2 start, Vector2 end, float diameter, Color col);

	bool DetectAppleCollisions();
};