#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "Scene.h"

#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <random>
#include <unordered_map>

# define EMPTY(...)
# define DEFER(...) __VA_ARGS__ EMPTY()
# define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
# define EXPAND(...) __VA_ARGS__

#define randColor {rand() % 1000 * .001f, rand() % 1000 * .001f, rand() % 1000 * .001f, 1}
#define refreshRate 100

//Memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
PxSceneDesc* sceneDesc = NULL;
ContactReportCallback gContactReportCallback;

Scene* mScene = nullptr;

int pxID = LAST_SCENE;
vector<RenderItem*> renderItems;
unordered_map<PhysicMaterial, PxMaterial*> gMaterials;
unordered_map<PhysicMaterial, PxVec4> colors;

// FPS
clock_t startTime = 0;
double fps;

PxRigidStatic* AddPxStatic(PxVec3 pos, PxShape* shape, PxVec4 color, PhysicMaterial material = DEFAULT) {
	shape->setMaterials(&gMaterials[material], 1);
	PxRigidStatic* particle = gPhysics->createRigidStatic(PxTransform(pos));
	particle->attachShape(*shape);
	renderItems.push_back(new RenderItem(shape, particle, color));
	gScene->addActor(*particle);
	return particle;
}

PxRigidDynamic* AddPxDynamic(PxVec3 pos, PxShape* shape, PxVec4 color, PhysicMaterial material = DEFAULT) {
	shape->setMaterials(&gMaterials[material], 1);
	PxRigidDynamic* particle = gPhysics->createRigidDynamic(PxTransform(pos));
	particle->attachShape(*shape);
	renderItems.push_back(new RenderItem(shape, particle, color));
	gScene->addActor(*particle);
	return particle;
}

void LoadScene(int newID) {
	if (gScene != NULL) {
		gScene->release();

		for(auto item : renderItems) {
			DeregisterRenderItem(item);
			delete item;
		}

		renderItems.clear();
	}
	gScene = gPhysics->createScene(*sceneDesc);

	pxID = newID;

	switch (pxID) {
	case 13:
		AddPxStatic({ 0, 30, 0 }, CreateShape(PxBoxGeometry(100, 1, 100)), { .42, .23, .16, 1 }, SOAP);
		AddPxStatic({ -1, 40, -1 }, CreateShape(PxBoxGeometry(.5, 11, .5)), { .3, .3, .3, 1 });
		break;
	default:
		if(pxID != -1)
			cout << "Scene " << pxID << " doesn't exist.\n";
		return;
	}

	//Mensaje
	cout << "Scene " << pxID << " loaded.\n";
	switch (pxID) {
	default:
		break;
	}
}

void updateWindowName(clock_t endTime) {
	clock_t deltaTime = endTime - startTime;

	fps++;

	if (deltaTime > refreshRate) {
		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << fps / (deltaTime * .001);
		std::string s = stream.str();

		std::string name1 = "Simulacion Fisica Videojuegos (FPS: " + s + ")";

		glutSetWindowTitle(name1.c_str());

		fps = 0;
		startTime = endTime;
	}
}

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterials[DEFAULT] = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	colors[DEFAULT] = { 1, 1, 1, 1 };
	gMaterials[RUBBER] = gPhysics->createMaterial(0.6f, 0.4f, 0.99f);
	colors[RUBBER] = { 1, 0, 0, 1 };
	gMaterials[METAL] = gPhysics->createMaterial(0.8f, 0.8f, 0.2f);
	colors[METAL] = { 0, 1, 0, 1 };
	gMaterials[SOAP] = gPhysics->createMaterial(0.05f, 0.02f, 0.5f);
	colors[SOAP] = { 0, 0, 1, 1 };

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	sceneDesc = new PxSceneDesc(gPhysics->getTolerancesScale());
	sceneDesc->gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc->cpuDispatcher = gDispatcher;
	sceneDesc->filterShader = contactReportFilterShader;
	sceneDesc->simulationEventCallback = &gContactReportCallback;
	sceneDesc->gravity = { 0, -9.8, 0 };
	
	LoadScene(pxID);
	mScene = DBG_NEW Scene();
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	switch (pxID) {
	case 13:
	{
		const double spawnTime = .2;
		const int maxParticles = 500;

		static double lastSpawn = 0;
		static int particles = 0;
		if (lastSpawn > spawnTime && particles < maxParticles) {
			const PxVec3 size(.5, .2, .3);
			PhysicMaterial material = PhysicMaterial(rand() % LAST_PXMATERIAL);
			float tensorMultiplier = (rand() % 100) * .1;

			PxRigidDynamic* particle = AddPxDynamic({ 0, 50, 0 },
				CreateShape(PxBoxGeometry(size)), colors[material] * tensorMultiplier * .1, material);

			particle->setMassSpaceInertiaTensor(
				PxVec3(size.y * size.z, size.x * size.z, size.x * size.y) * tensorMultiplier);
			particle->setLinearVelocity({ 10, 2, 0 });
			particles++;
			lastSpawn = 0;
		}
		else
			lastSpawn += t;
	}
		break;
	default:
		break;
	}

	mScene->Update(t);

	clock_t endTime = clock();

	updateWindowName(endTime);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	mScene->ClearScene();
	delete mScene;

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	delete sceneDesc;

	OutputDebugString("-----------_CrtDumpMemoryLeaks ---------\n");
	_CrtDumpMemoryLeaks();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	pxID = -1;

	switch(toupper(key))
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		mScene->LoadScene(key - '0');
		LoadScene(-1);
		return;
	case '\'':
		mScene->LoadScene(10);
		LoadScene(-1);
		return;
	case 'R':
		mScene->LoadScene(11);
		LoadScene(-1);
		return;
	case 'T':
		mScene->LoadScene(12);
		LoadScene(-1);
		return;
	case 'Y':
		mScene->LoadScene(-1);
		LoadScene(13);
		return;
	default:
		break;
	}

	mScene->KeyPress(key, camera);
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}