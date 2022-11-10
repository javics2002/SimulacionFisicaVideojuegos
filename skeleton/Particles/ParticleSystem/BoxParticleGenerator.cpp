#include "BoxParticleGenerator.h"

BoxParticleGenerator::BoxParticleGenerator(Particle* p, int num, 
	PxVec3 ori, PxVec3 dim, PxVec3 velVar, ForceRegistry* forceRegistry) 
	: ParticleGenerator(p, forceRegistry), particleNum(num), origin(ori), 
	dimensions(dim), velocityVariation(velVar)
{
	normalX = normal_distribution<float>(p->GetVel().x, velocityVariation.x);
	normalY = normal_distribution<float>(p->GetVel().y, velocityVariation.y);
	normalZ = normal_distribution<float>(p->GetVel().z, velocityVariation.z);
	uniformX = uniform_real_distribution<float>(origin.x - dimensions.x, origin.x + dimensions.x);
	uniformY = uniform_real_distribution<float>(origin.y - dimensions.y, origin.y + dimensions.y);
	uniformZ = uniform_real_distribution<float>(origin.z - dimensions.z, origin.z + dimensions.z);
}

vector<Particle*> BoxParticleGenerator::GenerateParticles()
{
	vector<Particle*> particles = vector<Particle*>();

	if (firstFrame) {
		for (int i = 0; i < particleNum; i++) {
			Particle* newParticle = new Particle(prefab);
			newParticle->SetPos(PxVec3(uniformX(generator), uniformY(generator), uniformZ(generator)));
			newParticle->SetVel(PxVec3(normalX(generator), normalY(generator), normalZ(generator)));

			particles.push_back(newParticle);
		}

		firstFrame = false;
	}

	return particles;
}
