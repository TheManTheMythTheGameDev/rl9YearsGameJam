#pragma once
#include "raylib.h"
#include "physics_object.h"
#include "reasings.h"

class HookSnake : public PhysicsObject
{
public:
	HookSnake(Vector2 position = Vector2{ 0.0f, 0.0f }, float len = 56.0f);

	void Update();
	void Draw();

	std::vector<int> controls[3];
private:
	float length;
	Vector2 tailPos;
	bool attached;
	bool attachedLastFrame;
	float angle;
	float startAngle;
	float targetAngle;
	float swingTimer;

	Vector2 lastPos;
	Vector2 curPos;
	Vector2 releaseVel;
};