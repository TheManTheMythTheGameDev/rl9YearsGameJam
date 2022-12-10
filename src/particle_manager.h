#pragma once
#include "particle.h"
#include <vector>

extern std::vector<Particle*> particles;

void InitParticles();

void AddParticle(Particle* target);

void LaunchParticles(Vector2 position);

void UpdateParticles(float dt);
void DrawParticles();

void UnloadParticles();