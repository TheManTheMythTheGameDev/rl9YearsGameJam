#pragma once
#include "raylib.h"
#include "raymath.h"

class Particle
{
public:
	// Direction should be in degrees, lifespan in seconds
	Particle(Vector2 position = Vector2{ 0.0f, 0.0f }, float direction = 0.0f, float lifespan = 1.0f, float initialVel = 5.0f, Vector2 gravity = Vector2{ 0.0f, 0.2f }, float radius = 1.0f);

	// Returns whether particle should be destroyed
	bool Update(float dt);
	void Draw();

	float lifeTimer, r, deleteTime;
	Vector2 pos, vel, grav;
	Color col; // Random
};