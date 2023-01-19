#pragma once

#include "Particles/Projectile.h"
#include "Particles/ParticleManager.h"
#include "Force/ForceRegistry.h"
#include "Force/ForceRegistryRigid.h"

#include "callbacks.hpp"

#include <random>
using namespace std;
using namespace physx;

//Physx
extern PxScene* gScene;
extern PxSceneDesc* sceneDesc;
extern PxPhysics* gPhysics;
extern PxFoundation* gFoundation;
extern PxPvd* gPvd;
extern PxDefaultCpuDispatcher* gDispatcher;

extern PxDefaultAllocator gAllocator;
extern PxDefaultErrorCallback gErrorCallback;
extern ContactReportCallback gContactReportCallback;

extern unordered_map<PhysicMaterial, physx::PxMaterial*> gMaterials;

constexpr int LAST_SCENE = 17;

class Contact;
class Link;
class Pinball;

class Scene
{
	int mID = 0;

	random_device rand;
	default_random_engine generator = default_random_engine(rand());

	//Scene 11
	double AddToAllSprings(double value, bool k);
	void ShowSpringsValue(double value, bool k);

	unordered_map<PhysicMaterial, PxVec4> colors;
	vector<RenderItem*> renderItems;

	//Constraints
	vector<Contact*> contacts;
	vector<Link*> links;

	//Proyecto final
	Pinball* pinball = nullptr;

public:
	Scene();
	~Scene();

	void LoadScene(int newID);
	void Update(double t);
	void ClearScene();

	ParticleManager particles;

	ForceRegistry fr;
	ForceRegistryRigid frr;
	vector<ForceGenerator*> fg;

	void KeyPress(unsigned char key, const PxTransform& camera);
	void ThrowProyectile(ProjectileType type, const PxTransform& camera);

	//Physx
	PxRigidStatic* AddPxStatic(PxVec3 pos, PxShape* shape, PxVec4 color, 
		PhysicMaterial material = DEFAULT);

	void onCollision(PxActor* actor1, PxActor* actor2);
};

