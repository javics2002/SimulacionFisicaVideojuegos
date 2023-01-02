#include "Scene.h"

#include <ctype.h>
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <random>

# define EMPTY(...)
# define DEFER(...) __VA_ARGS__ EMPTY()
# define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
# define EXPAND(...) __VA_ARGS__

//#define randColor {rand() % 1000 * .001f, rand() % 1000 * .001f, rand() % 1000 * .001f, 1}
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

PxScene* gScene = nullptr;
PxSceneDesc* sceneDesc = nullptr;
PxPhysics* gPhysics = nullptr;
PxFoundation* gFoundation = nullptr;
PxPvd* gPvd = nullptr;
PxDefaultCpuDispatcher* gDispatcher = nullptr;

PxDefaultAllocator gAllocator;
PxDefaultErrorCallback gErrorCallback;
ContactReportCallback gContactReportCallback;

unordered_map<PhysicMaterial, physx::PxMaterial*> gMaterials;

Scene* mScene = nullptr;

// FPS
clock_t startTime = 0;
double fps;

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
	
	mScene = DBG_NEW Scene();
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	mScene->Update(t);

	clock_t endTime = clock();

	updateWindowName(endTime);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	delete mScene;

	OutputDebugString("-----------_CrtDumpMemoryLeaks ---------\n");
	_CrtDumpMemoryLeaks();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

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
		return;
	case '\'':
		mScene->LoadScene(10);
		return;
	case 'R':
		mScene->LoadScene(11);
		return;
	case 'T':
		mScene->LoadScene(12);
		return;
	case 'Y':
		mScene->LoadScene(13);
		return;
	case 'U':
		mScene->LoadScene(14);
		return;
	case 'I':
		mScene->LoadScene(15);
		return;
	case 'O':
		mScene->LoadScene(16);
		return;
	case 'P':
		mScene->LoadScene(17);
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