#include "Firework.h"
#include "ParticleSystem/ParticleSystem.h"
#include "../RenderUtils.hpp"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

Firework::Firework(ParticleSystem* sys, FireworkType t, PxVec3 spawnPos) 
	: Particle(spawnPos), system(sys), type(t)
{
	colorDistribution = uniform_int_distribution<>(0, LAST - 1);

	CreateFirework(type);
}

Firework::Firework(Firework* f) : Particle(f), system(f->system), type(f->type)
{
	generations = f->generations - 1;
	divitionsMultiplier = f->divitionsMultiplier;
	divitions = f->divitions * divitionsMultiplier;
	blastMultiplier = f->blastMultiplier;
	blastForce = f->blastForce * blastMultiplier;
	colorDistribution = f->colorDistribution;
	lifetimeDistribution = f->lifetimeDistribution;
	color = f->color;
	sizeMultiplier = f->sizeMultiplier;
	size = f->size * sizeMultiplier;

	if (type == GORDO)
		SetLifetime(0.2f * lifetimeDistribution(generator));
	else if (type ==FOUNTAIN_FIREWORK)
		SetLifetime(3 * lifetimeDistribution(generator));
	else
		SetLifetime(lifetimeDistribution(generator));
	if(type != COLLISION)
		SetColor(fireworkColor[color]);
	SetShape(CreateShape(PxSphereGeometry(size)));
}

Firework::~Firework() noexcept
{
	if (shape != nullptr && generations > 0) {
		for (int i = 0; i < divitions; i++) {
			Firework* f = DBG_NEW Firework(this);
			f->SetVel(PxVec3(GetVel().x + velocityDistribution(generator) * blastForce,
				GetVel().y + velocityDistribution(generator) * blastForce,
				GetVel().z + velocityDistribution(generator) * blastForce));

			system->particles.AddSafe(f);
		}
	}
}

void Firework::CreateFirework(FireworkType type)
{
	switch (type)
	{
	default:
	case BROCADE:
		generations = 3;
		divitions = 20;
		divitionsMultiplier = .55f;
		lifetimeDistribution = normal_distribution<>(1, .2);
		velocityDistribution = normal_distribution<>(0, 1);
		blastForce = 5;
		blastMultiplier = .75f;

		SetVel(PxVec3(velocityDistribution(generator), 50 + velocityDistribution(generator), velocityDistribution(generator)));
		SetAcc({ 0, -10, 0 });
		SetDamp(.7);
		break;
	case CHRYSANTHEMUM:
		generations = 4;
		divitions = 50;
		divitionsMultiplier = .3f;
		lifetimeDistribution = normal_distribution<>(1.5, .3);
		velocityDistribution = normal_distribution<>(0, 1);
		blastForce = 6;
		blastMultiplier = .6f;

		SetVel(PxVec3(velocityDistribution(generator), 40 + velocityDistribution(generator), velocityDistribution(generator)));
		SetAcc({ 0, -10, 0 });
		SetDamp(.7);
		break;
	case COMET:
		generations = 10;
		divitions = 2;
		divitionsMultiplier = 1;
		lifetimeDistribution = normal_distribution<>(.3, .1);
		velocityDistribution = normal_distribution<>(0, 1);
		blastForce = 1.0f;
		blastMultiplier = 1;

		SetVel(PxVec3(velocityDistribution(generator), 50 + velocityDistribution(generator), velocityDistribution(generator)));
		SetAcc({ 0, -10, 0 });
		SetDamp(.7);
		break;
	case DELAY_CRACKLE:
		generations = 2;
		divitions = 100;
		divitionsMultiplier = .05f;
		lifetimeDistribution = normal_distribution<>(2, .5);
		velocityDistribution = normal_distribution<>(0, 5);
		blastForce = 1.0f;
		blastMultiplier = .1f;

		SetVel(PxVec3(velocityDistribution(generator), 40 + velocityDistribution(generator), velocityDistribution(generator)));
		SetAcc({ 0, -10, 0 });
		SetDamp(.7);
		break;
	case GORDO:
		generations = 1;
		divitions = 1000;
		lifetimeDistribution = normal_distribution<>(5, 2);
		velocityDistribution = normal_distribution<>(0, 5);
		blastForce = 20.0f;
		size = .5f;

		SetVel(PxVec3(velocityDistribution(generator), 55 + velocityDistribution(generator), velocityDistribution(generator)));
		SetAcc({ 0, -10, 0 });
		SetDamp(.7);
		break;
	case FOUNTAIN_FIREWORK:
		generations = 1;
		divitions = 50;
		lifetimeDistribution = normal_distribution<>(2, .3);
		velocityDistribution = normal_distribution<>(0, .1);
		blastForce = 10.0f;
		blastMultiplier = 1.0f;

		SetVel(PxVec3(velocityDistribution(generator), 30 + velocityDistribution(generator), velocityDistribution(generator)));
		SetAcc({ 0, -10, 0 });
		SetDamp(.98);
		break;
	case COLLISION:
		generations = 1;
		divitions = 10;
		divitionsMultiplier = 0;
		lifetimeDistribution = normal_distribution<>(.4, .1);
		velocityDistribution = normal_distribution<>(0, .1);
		blastForce = 1.0f;
		blastMultiplier = .1f;
		size = 0.01f;
		SetLifetime(0);

		SetVel(PxVec3(velocityDistribution(generator), velocityDistribution(generator), velocityDistribution(generator)));
		SetDamp(.2);
		break;
	}

	SetShape(CreateShape(PxSphereGeometry(size)));

	if (type != COLLISION) {
		color = colorDistribution(generator);
		SetColor(fireworkColor[color]);
		SetLifetime(lifetimeDistribution(generator));
		static uniform_real_distribution<float> spawn = uniform_real_distribution<float>(-20, 20);
		SetPos({ spawn(generator), 0, spawn(generator) });
	}
}