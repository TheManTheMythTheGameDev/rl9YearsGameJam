#pragma once
#include "raylib.h"
#include "physics_object.h"
#include "reasings.h"
#include "apple_manager.h"
#include "particle_manager.h"

class HookSnake : public PhysicsObject
{
public:
	HookSnake(Vector2 position = Vector2{ 0.0f, 0.0f }, float len = 56.0f);

	// Returns whether snake is dead
	bool Update(Camera2D cam, float dt);
	void Draw();

	Vector2 GetLastCheckpoint();

	std::vector<int> controls[3];
private:
	float length;
	Vector2 tailPos;
	bool attached;
	bool attachedLastFrame;
	float angle;

	float swingVel;
	float swingLen;
	bool resolvingCollision;

	Vector2 lastPos;
	Vector2 curPos;
	Vector2 releaseVel;

	Vector2 startPos;
	Vector2 posChange;

	Vector2 lastCheckpoint;
	Vector2 checkpointBeforeLast;

	bool DetectAppleCollisions();
};