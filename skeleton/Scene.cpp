#include "Scene.h"
#include "RenderUtils.hpp"
#include <iostream>

Scene::Scene()
{
	LoadScene(0);
	cout << "Pulsa los numeros para cambiar entre las escenas.\n"
		<< "0: Particula con velocidad constante\n"
		<< "1: Particula con aceleracion y damping\n"
		<< "2: Proyectiles\n";
}

Scene::~Scene()
{
	ClearScene();
}

void Scene::LoadScene(int newID)
{
	ClearScene();

	mID = newID;

	switch (mID) {
	case 0:
		AddParticle((new Particle())->SetVel({ 0, 10, 0 })->SetColor({.98, .96, .3, 1.0}));
		break;
	case 1:
		AddParticle((new Particle())->SetAcc({0, 2, 0})->SetDamp(.9)->SetColor({.31, .81, .96, 1.0}));
		break;
	case 2:
		cout << "Puedes disparar proyectiles.\n";
			/*<< "0: Particula con velocidad constante\n"
			<< "1: Particula con aceleracion y damping\n"
			<< "2: Proyectiles\n";*/
		AddParticle((new Particle())->SetColor({.93, .81, .61, 1.0})->SetShape(CreateShape(PxBoxGeometry(100, .5, 100))));
		break;
	default:
		cout << "Scene " << mID << " doesn't exist.\n";
		return;
	}

	cout << "Scene " << mID << " loaded.\n";
}

void Scene::Update(double t)
{
	for (auto p : mParticles)
		p->Integrate(t);
}

int Scene::AddParticle(Particle* p)
{
	if (p != nullptr) {
		mParticles.push_back(p);
		return mParticles.size() - 1;
	}
	else return -1;
}

Particle* Scene::GetParticle(int id)
{
	if (id >= 0 && id < mParticles.size())
		return mParticles[id];
	else return nullptr;
}

bool Scene::RemoveParticle(int id)
{
	if (id < 0 && id >= mParticles.size())
		return false;

	vector<Particle*>::iterator it = mParticles.begin();
	int i = 0;
	while (i < id) {
		it++;
		i++;
	}
	
	mParticles.erase(it);
	return true;
}

void Scene::ClearScene()
{
	for (auto p : mParticles)
		delete p;

	mParticles.clear();
}

void Scene::KeyPress(unsigned char key, const physx::PxTransform& camera)
{
	switch (mID) {
	case 2:
		switch (toupper(key)) {
		case 'Q':
			throwProyectile(PISTOL, camera);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Scene::throwProyectile(ProjectileType type, const physx::PxTransform& camera)
{
	//Crea el proyectil delante de la camara
	Particle* p = new Particle(camera.p - 2 * camera.q.getBasisVector2());
	double vel; 
	switch (type) {
	case PISTOL:
		break;
	case ARTILLERY:
		break;
	case FIREBALL:
		break;
	case LASER:
		break;
	default:
		return;
	}

	//Se lanza hacia el frente
	AddParticle(p->SetVel(-camera.q.getBasisVector2()));
}
