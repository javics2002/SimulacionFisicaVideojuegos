#include "Firework.h"

Firework::Firework(FireworkType t) : Particle(), type(t)
{
	uniform = uniform_int_distribution<>(0, LAST);

	CreateFirework(type, true);
}

Firework::Firework(Firework* f) : Particle(f), type(f->type)
{
	generations = f->generations - 1;
	uniform = f->uniform;
	normal = f->normal;

	CreateFirework(type, false);
}

Firework::~Firework()
{
	if (generations > 0) {
		for (int i = 0; i < divitions; i++)
			Firework(this);
	}
}

void Firework::CreateFirework(FireworkType type, bool setGenerations)
{
	switch (type)
	{
	case BROCADE:
		if (setGenerations) {
			generations = 4;
			normal = normal_distribution<>(1, .2);
		}
		break;
	case CHRYSANTHEMUM:
		if (setGenerations) {
			generations = 6;
			normal = normal_distribution<>(1.5, .3);
		}
		break;
	case COMET:
		if (setGenerations) {
			generations = 10;
			normal = normal_distribution<>(.7, .1);
		}
		break;
	default:
		break;
	}

	SetLifetime(normal(generator));
	SetColor(fireworkColor[uniform(generator)]);
}