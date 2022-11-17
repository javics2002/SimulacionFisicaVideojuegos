#include "BoxParticleGenerator.h"
#include "../../RenderUtils.hpp"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

BoxParticleGenerator::BoxParticleGenerator(Particle* p, int num, 
	PxVec3 ori, PxVec3 dim, PxVec3 velVar, double invMassVar, ForceRegistry* forceRegistry)
	: ParticleGenerator(p, forceRegistry), particleNum(num), origin(ori), 
	dimensions(dim), velocityVariation(velVar), invMassVariation(invMassVar)
{
	normalX = normal_distribution<float>(p->GetVel().x, velocityVariation.x);
	normalY = normal_distribution<float>(p->GetVel().y, velocityVariation.y);
	normalZ = normal_distribution<float>(p->GetVel().z, velocityVariation.z);
	uniformX = uniform_real_distribution<float>(origin.x - dimensions.x, origin.x + dimensions.x);
	uniformY = uniform_real_distribution<float>(origin.y - dimensions.y, origin.y + dimensions.y);
	uniformZ = uniform_real_distribution<float>(origin.z - dimensions.z, origin.z + dimensions.z);
	lognormalMass = lognormal_distribution<>(p->GetInvMass(), invMassVariation);
}

double lerp(double a, double b, double f)
{
	return a + f * (b - a);
}

vector<Particle*> BoxParticleGenerator::GenerateParticles()
{
	vector<Particle*> particles = vector<Particle*>();

	if (firstFrame) {
		for (int i = 0; i < particleNum; i++) {
			Particle* newParticle = DBG_NEW Particle(prefab);
			newParticle->SetPos(PxVec3(uniformX(generator), uniformY(generator), uniformZ(generator)));
			newParticle->SetVel(PxVec3(normalX(generator), normalY(generator), normalZ(generator)));
			newParticle->SetIMass(lognormalMass(generator));
			double size = .2 / newParticle->GetInvMass();
			newParticle->SetShape(CreateShape(PxSphereGeometry(size)));

			particles.push_back(newParticle);
		}

		firstFrame = false;
	}

	return particles;
}
