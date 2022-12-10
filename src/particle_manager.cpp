#include "particle_manager.h"

std::vector<Particle*> particles;

void InitParticles()
{
	particles = std::vector<Particle*>();
}

void AddParticle(Particle* target)
{
	particles.push_back(target);
}

void LaunchParticles(Vector2 position)
{
	// Launch semicircle of particles into the air
	for (int i = 0; i > -180; i--)
	{
		Particle* cur = new Particle(position, i);
		AddParticle(cur);
	}
}

void UpdateParticles(float dt)
{
	for (int i = particles.size() - 1; i >= 0; i--)
	{
		if (particles[i]->Update(dt))
		{
			delete particles[i];
			particles.erase(particles.begin() + i);
		}
	}
}

void DrawParticles()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->Draw();
	}
}

void UnloadParticles()
{
	for (int i = particles.size() - 1; i >= 0; i--)
	{
		delete particles[i];
		particles.erase(particles.begin() + i);
	}
}
