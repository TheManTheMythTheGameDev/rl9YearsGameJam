#pragma once
#include "raylib.h"
#include "grid.h"
#include "physics_object.h"
#include <vector>
#include <algorithm>

class Snake : public PhysicsObject
{
public:
	Snake(Vector2 position = Vector2{ 0.0f, 0.0f }, float spacing = 8.0f);

	void Update();
	void Draw();

	std::vector<int> controls[3];

	// index 0 = head
	std::vector<Vector2> nodes;
private:
	float nodeSpacing;
	int curRotNode;
	bool turnDir;
};