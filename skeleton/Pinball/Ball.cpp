#include "Ball.h"
#include "../Particles/ParticleSystem/ParticleSystems.h"
#include "../Force/Whirlwind.h"
#include "../Scene.h"

Ball::Ball(Scene* scene) : RigidParticle(startPos, .04, CreateShape(PxSphereGeometry(.02)),
	{.8, .8, .8, 1}, BALL)
{
	trail = CreateGenerator(TRAIL, &scene->fr);
	system = new ParticleSystem(&scene->fr, trail);
	particle->setMassSpaceInertiaTensor(PxVec3(2 * particle->getMass() * .02 * .02 / 5));
}

Ball::~Ball()
{
	if (system != nullptr)
		delete system;
}

void Ball::Reset()
{
	particle->setGlobalPose(PxTransform(startPos));
	particle->setLinearVelocity(PxVec3(0));
}

void Ball::Integrate(double t)
{
	trail->prefab->SetPos(particle->getGlobalPose().p);
	system->Integrate(t);
}

void Ball::ToggleGamer()
{
	gamer = !gamer;

	if (gamer) {
		//Bola hueca pero mas pesada
		particle->setMass(.2);
		particle->setMassSpaceInertiaTensor(PxVec3(2 * particle->getMass() * .02 * .02 / 3));
		SetColor({ 0,1,1,1 });
	}
	else {
		//Bola maciza de metal
		particle->setMass(.04);
		particle->setMassSpaceInertiaTensor(PxVec3(2 * particle->getMass() * .02 * .02 / 5));
		SetColor({ .8, .8, .8, 1 });
	}
}

void Ball::ToggleTornado()
{
	tornado = !tornado;

	if (tornado)
		trail->AddForceGenerator(std::make_shared<Whirlwind>(PxVec3(0, 2, 0), 0.02, .1));
	else
		trail->ClearForceGenerators();
}
