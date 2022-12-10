#include "particle.h"

Particle::Particle(Vector2 position, float direction, float lifespan, float initialVel, Vector2 gravity, float radius)
{
	pos = position;
	lifeTimer = 0.0f;
	deleteTime = lifespan;
	grav = gravity;
	r = radius;

	vel = Vector2{ cosf(direction * DEG2RAD), sinf(direction * DEG2RAD) };
	vel = Vector2Scale(vel, initialVel);

	int r, g, b;
	r = GetRandomValue(0, 255);
	g = GetRandomValue(0, 255);
	b = GetRandomValue(0, 255);
	col = Color{ (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 };
}

bool Particle::Update(float dt)
{
	lifeTimer += dt;

	vel = Vector2Add(vel, grav);
	vel = Vector2Scale(vel, 0.99f);

	pos = Vector2Add(pos, Vector2Scale(vel, 60.0f * dt));

	float a = Lerp(0, 255, lifeTimer / deleteTime);
	a = 255 - a;
	col.a = (unsigned char)a;

	if (lifeTimer >= deleteTime)
	{
		return true;
	}

	return false;
}

void Particle::Draw()
{
	DrawCircle(pos.x, pos.y, r, col);
}
