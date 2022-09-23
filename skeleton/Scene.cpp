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
		cout << "Puedes disparar proyectiles.\n"
			<< "Z: Bala\n"
			<< "X: Artilleria\n"
			<< "C: Bola de fuego\n"
			<< "V: Laser\n";

		//Suelo
		AddParticle((new Particle({0, 48.5, 0}))->SetColor({.93, .81, .61, 1.0})
			->SetShape(CreateShape(PxBoxGeometry(1000, .5, 1000))));
		
		//Vallas
		AddParticle((new Particle({50, 48.5, 40}))->SetColor({.2, .6, .9, 1.0})
			->SetShape(CreateShape(PxBoxGeometry(10, 1, .5))));
		AddParticle((new Particle({40, 48.5, 50}))->SetColor({.2, .6, .9, 1.0})
			->SetShape(CreateShape(PxBoxGeometry(.5, 1, 10))));

		//Diana
		AddParticle((new Particle({ 42, 50, 42 }))->SetColor({ .8, .5, .4, 1.0 })
			->SetShape(CreateShape(PxSphereGeometry(.6))));
		break;
	default:
		cout << "Scene " << mID << " doesn't exist.\n";
		return;
	}

	cout << "Scene " << mID << " loaded.\n";
}

void Scene::Update(double t)
{
	for (auto p : mParticles) {
		p->Integrate(t);

		//Borrar particulas que se hayan ido lejos
	}
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
		case 'Z':
			ThrowProyectile(PISTOL, camera);
			break;
		case 'X':
			ThrowProyectile(ARTILLERY, camera);
			break;
		case 'C':
			ThrowProyectile(FIREBALL, camera);
			break;
		case 'V':
			ThrowProyectile(LASER, camera);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Scene::ThrowProyectile(ProjectileType type, const physx::PxTransform& camera)
{
	PxVec3 front = -camera.q.getBasisVector2().getNormalized();
	PxVec3 up = -camera.q.getBasisVector0().getNormalized();

	//Crea el proyectil delante de la camara
	Particle* p = new Particle(camera.p + 1.5 * front);
	Projectile real, simulated;
	real.initialHeight = camera.p.y;
	real.angle = 0;

	switch (type) {
	case PISTOL:
		p->SetShape(CreateShape(PxSphereGeometry(0.1)));
		p->SetColor({ .7, .7, .7, 1 });

		//Masa real: 0.18 kg	Velocidad real: 360 m/s
		real.inverseMass = 1 / .18;
		real.speed = 360;
		real.gravity = g;

		//Velocidad deseada: 40 m/s
		simulated = Simulate(40, real);
		break;
	case ARTILLERY:
		p->SetShape(CreateShape(PxSphereGeometry(.5)));
		p->SetColor({ .2, .2, .2, 1 });

		//Masa real: 50 kg	Velocidad real: 1800 m/s
		real.inverseMass = 1 / 50.0;
		real.speed = 1800;
		real.gravity = g;

		//Velocidad deseada: 20 m/s
		simulated = Simulate(20, real);
		break;
	case FIREBALL:
		p->SetShape(CreateShape(PxSphereGeometry(0.2)));
		p->SetColor({ 1, .4, .1, 1 });

		//Masa real: 2 kg	Velocidad real: 10 m/s
		real.inverseMass = 1 / 2.0;
		real.speed = 10;
		real.gravity = 1;

		//Velocidad deseada: 10 m/s
		simulated = Simulate(10, real);
		break;
	case LASER:
		p->SetShape(CreateShape(PxSphereGeometry(0.1)));
		p->SetColor({ 1, .3, .3, 1 });

		//Masa real: 0 kg	Velocidad real: c m/s
		real.inverseMass = DBL_MAX;
		real.speed = c;
		real.gravity = 0;

		//Velocidad deseada: 50 m/s
		simulated = Simulate(50, real);
		break;
	default:
		return;
	}
	
	//Se lanza hacia el frente
	p->SetVel(PxCos(simulated.angle) * simulated.speed * front + PxSin(simulated.angle) * simulated.speed * up);

	p->SetAcc({ 0, (float) simulated.gravity, 0 });
	p->SetIMass(simulated.inverseMass);

	cout << "Masa real: " << 1 / real.inverseMass << " kg	Velocidad real: " << real.speed << " m/s	Gravedad real: " << real.gravity << " m/s^2\n"
		<< "Masa simulada: " << 1 / simulated.inverseMass << " kg	Velocidad simulada: " << simulated.speed << " m/s	Gravedad simulada: " << simulated.gravity << " m/s^2\n";

	AddParticle(p);
}

Scene::Projectile Scene::Simulate(double simulatedVel, Projectile real)
{
	Projectile simulated;
	simulated.speed = simulatedVel;
	simulated.initialHeight = real.initialHeight;
	simulated.angle = real.angle;

	//Conserva la energia	E = mv
	simulated.inverseMass = real.inverseMass * pow(simulated.speed / real.speed, 2);

	//Queremos que recorra la misma distancia	0 = p + vt + .5 * at^2
	if (real.gravity != 0) {
		double realFlightTime = sqrt(2 * real.initialHeight / abs(real.gravity));
		double distanceReached = real.speed * realFlightTime;
		double simulatedFlightTime = distanceReached / simulated.speed;

		simulated.gravity = real.gravity * pow(realFlightTime, 2) / pow(simulatedFlightTime, 2);
	}
	else
		simulated.gravity = 0;

	return simulated;
}
