#pragma once

#include "Particle.h"
#include <vector>
#include <random>

enum FireworkType { BROCADE, CHRYSANTHEMUM, COMET };
enum FireworkColor { RED, ORANGE, YELLOW, LIME, GREEN, CYAN, BLUE, VIOLET, MAGENTA, GOLD, WHITE, LAST };
static vector<PxVec4> fireworkColor = { {1, 0, 0, 1}, {1, .5, 0, 1}, {1, 1, 0, 1}, {.7, 1, .3, 1}, 
	{0, 1, 0, 1}, {0, 1, 1, 1}, {0, 0, 1, 1}, {.6, .2, .8, 1}, {1, 0, 1, 1}, {.6, .5, 0, 1}, {1, 1, 1, 1} };

class Firework : public Particle
{
	int generations; //numero de veces que se divide un fuego artificial
	int divitions; //cuantas particulas genera cada division
	FireworkType type;

	double lifetimeVariation;

	random_device rand;
	default_random_engine generator = default_random_engine(rand());
	uniform_int_distribution<> uniform;
	normal_distribution<> normal;

	void CreateFirework(FireworkType t, bool setGenerations);

public:
	Firework(FireworkType type);
	Firework(Firework* f);
	~Firework();
};