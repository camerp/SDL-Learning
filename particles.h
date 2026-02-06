#pragma once

#include <SDL3\SDL.h>
#include "Utility.h"
#include "SIMD.h"

#define MAX_PARTICLES 16000


struct ParticleSystem
{
	SDL_FPoint points[MAX_PARTICLES];
	SDL_FPoint particles[MAX_PARTICLES];
	Color32 color;
	SDL_FPoint origin = { 0,0 };
	float friction = 0.99f;
	float gravity = 0.1f;
	Uint32 particleIndex = 0;
	Uint32 count = 0;
	Uint32 padding;
};


void setupParticleSystem(ParticleSystem* ps)
{
	ps->friction = 0.99f;
	ps->gravity = 0.1f;
};


void QuickSpawnParticle(ParticleSystem* ps)
{
	Uint32 i = ps->particleIndex;
	SDL_FPoint* points = ps->points;
	SDL_FPoint* particle = ps->particles;
	float tf = SDL_randf() * 10;
	SDL_FPoint vel = { (float)fma(SDL_randf(), 2.0f, -1.0f) * tf, (float)fma(SDL_randf(), 2.0f, -1.0f) * tf };
	points[i] = ps->origin;
	particle[i] = vel;
	if (i < MAX_PARTICLES-1)
		ps->particleIndex++;
	else
		ps->particleIndex = 0;

	if (ps->count < MAX_PARTICLES-1)
		ps->count++;
}

Uint64 UpdateParticles(ParticleSystem* ps)
{
	float friction = ps->friction;
	float gravity = ps->gravity;
	float grav[2] = { 0.0f, gravity };
	Uint32 max = ps->count * 2;
	float* points = (float*)ps->points;
	float* particles = (float*)ps->particles;
	SIMD::SetSSE();
	Uint32 i, j, k, l;
	i = 0;
	Uint8 step = SIMD::OpStep;
	Uint64 timer = SDL_GetTicksNS();
	while (i < max)
	{
		j = i + step;
		k = j + step;
		l = k + step;
		SIMD::SSE::particleUpdate(&points[i], &particles[i], friction, gravity);
		SIMD::SSE::particleUpdate(&points[j], &particles[j], friction, gravity);
		SIMD::SSE::particleUpdate(&points[k], &particles[k], friction, gravity);
		SIMD::SSE::particleUpdate(&points[l], &particles[l], friction, gravity);
		i = l + step;
	}
	return SDL_GetTicksNS() - timer;
}


void DrawParticleSystem(SDL_Renderer* r, ParticleSystem* ps)
{
	SDL_SetRenderDrawColor(r, ps->color.r, ps->color.g, ps->color.b, ps->color.a);
	SDL_RenderPoints(r, ps->points, ps->count);
}


