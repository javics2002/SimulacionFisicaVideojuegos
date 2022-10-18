#include "Firework.h"
#include "ParticleSystem/ParticleSystem.h"
#include "../RenderUtils.hpp"

Firework::Firework(ParticleSystem* sys, FireworkType t) : Particle(), system(sys), type(t)
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

	SetLifetime(lifetimeDistribution(generator));
	SetColor(fireworkColor[color]);
	SetShape(CreateShape(PxSphereGeometry(size)));
}

Firework::~Firework() noexcept
{
	if (generations > 0) {
		for (int i = 0; i < divitions; i++) {
			Firework* f = new Firework(this);
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
	default:
		break;
	}

	SetLifetime(lifetimeDistribution(generator));
	color = colorDistribution(generator);
	SetColor(fireworkColor[color]);
	SetShape(CreateShape(PxSphereGeometry(size)));

	static uniform_real_distribution<float> spawn = uniform_real_distribution<float>(-20, 20);
	SetPos({ spawn(generator), 0, spawn(generator) });
}