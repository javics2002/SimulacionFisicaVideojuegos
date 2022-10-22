#pragma once

#include "Particle.h"
#include <vector>
#include <random>

class ParticleSystem;

enum FireworkType { BROCADE, CHRYSANTHEMUM, COMET, DELAY_CRACKLE, GORDO, FOUNTAIN_FIREWORK };
enum FireworkColor { RED, ORANGE, YELLOW, LIME, GREEN, CYAN, BLUE, VIOLET, MAGENTA, GOLD, WHITE, LAST };
static vector<PxVec4> fireworkColor = { {1, 0, 0, 1}, {1, .5, 0, 1}, {1, 1, 0, 1}, {.7, 1, .3, 1}, 
	{0, 1, 0, 1}, {0, 1, 1, 1}, {0, 0, 1, 1}, {.6, .2, .8, 1}, {1, 0, 1, 1}, {.6, .5, 0, 1}, {1, 1, 1, 1} };

class Firework : public Particle
{
	int generations; //numero de veces que se divide un fuego artificial
	int divitions; //cuantas particulas genera cada division
	float divitionsMultiplier = .5f;
	float blastForce = 5;
	float blastMultiplier = .6f;
	FireworkType type;
	int color;
	float size = .3f;
	float sizeMultiplier = .9f;

	random_device rand;
	default_random_engine generator = default_random_engine(rand());
	uniform_int_distribution<> colorDistribution;
	normal_distribution<> lifetimeDistribution;
	normal_distribution<> velocityDistribution;

	ParticleSystem* system;

	void CreateFirework(FireworkType t);

public:
	Firework(ParticleSystem* sys, FireworkType t);
	Firework(Firework* f);
	~Firework() noexcept;
};