#pragma once
#include "raylib.h"
#include "physics_object.h"
#include <vector>
#include <algorithm>

class BadApple : public PhysicsObject
{
public:
	BadApple(Vector2 position = Vector2{ 0.0f, 0.0f }, Vector2 targetPosition = Vector2{ 0.0f, 0.0f });

	void Update(float dt);
	void Draw();

	void SetTargetPosition(PhysicsObject newPosition);
private:
	PhysicsObject targetPos;
	Vector2 pathfindTarget;
	Vector2 secondaryPathfindTarget;
	float speed;

	bool jumping;

	Vector2 lastGround;

	void Jump();
	void MoveLeft(float dt);
	void MoveRight(float dt);
};