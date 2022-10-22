#include "Scene.h"
#include "RenderUtils.hpp"
#include <iostream>

#include "Particles/Firework.h"
#include "Particles/ParticleSystem/ParticleSystems.h"

Scene::Scene()
{
	cout << "Pulsa los numeros para cambiar entre las escenas.\n"
		<< "0: Particula con velocidad constante\n"
		<< "1: Particula con aceleracion y damping\n"
		<< "2: Proyectiles\n"
		<< "3: Sistema de particulas\n"
		<< "4: Fuegos artificiales\n\n";

	LoadScene(LAST_SCENE);
}

Scene::~Scene()
{
}

void Scene::LoadScene(int newID)
{
	particles.Clear();

	mID = newID;

	switch (mID) {
	case 0:
		particles.Add((new Particle())->SetVel({ 0, 10, 0 })->SetColor({.98, .96, .3, 1.0}));
		break;
	case 1:
		particles.Add((new Particle())->SetAcc({0, 2, 0})->SetDamp(.9)->SetColor({.31, .81, .96, 1.0}));
		break;
	case 2:
		cout << "Puedes disparar proyectiles.\n"
			<< "Z: Bala\n"
			<< "X: Artilleria\n"
			<< "C: Bola de fuego\n"
			<< "V: Laser\n"
			<< "B: Pelota de golf\n"
			<< "N: Frisbee\n"
			<< "M: Globo\n";

		//Suelo
		particles.Add((new Particle({0, 48.5, 0}))->SetColor({.93, .81, .61, 1.0})
			->SetShape(CreateShape(PxBoxGeometry(1000, .5, 1000))));
		
		//Vallas
		particles.Add((new Particle({50, 48.5, 40}))->SetColor({.2, .6, .9, 1.0})
			->SetShape(CreateShape(PxBoxGeometry(10, 1, .5))));
		particles.Add((new Particle({40, 48.5, 50}))->SetColor({.2, .6, .9, 1.0})
			->SetShape(CreateShape(PxBoxGeometry(.5, 1, 10))));

		//Diana
		particles.Add((new Particle({ 42, 50, 42 }))->SetColor({ .8, .5, .4, 1.0 })
			->SetShape(CreateShape(PxSphereGeometry(.6))));
		break;
	case 3:
		cout << "Puedes cambiar las particulas.\n"
			<< "Z: Fuente\n"
			<< "X: Niebla\n"
			<< "C: Polvo\n"
			<< "V: Fuego\n"
			<< "B: Lluvia\n"
			<< "N: Chispas\n"
			<< "M: Explosion\n"
			<< ",: Nieve\n"
			<< ".: Borrar particulas\n";


		particles.Add(new ParticleSystem(CreateGenerator(FOUNTAIN)));
		break;
	case 4:
		cout << "Puedes lanzar fuegos artificiales.\n"
			<< "Z: Brocado\n"
			<< "X: Crisantemo\n"
			<< "C: Cometa\n"
			<< "V: Delay\n"
			<< "B: El gordo\n"
			<< "N: Fuente\n";

		particles.Add(new ParticleSystem());
		break;
	default:
		cout << "Scene " << mID << " doesn't exist.\n";
		return;
	}

	cout << "Scene " << mID << " loaded.\n";
}

void Scene::Update(double t)
{
	particles.Integrate(t);
}

void Scene::ClearScene()
{
	particles.Clear();
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
		case 'B':
			ThrowProyectile(GOLF, camera);
			break;
		case 'N':
			ThrowProyectile(FRISBEE, camera);
			break;
		case 'M':
			ThrowProyectile(BALLOON, camera);
			break;
		default:
			break;
		}
		break;
	case 3:
	{
		ParticleSystem* system = static_cast<ParticleSystem*>(particles.Get(0));
		system->particles.Clear();
		switch (toupper(key)) {
		case 'Z':
			system->ReplaceGenerators(CreateGenerator(FOUNTAIN));
			break;
		case 'X':
			system->ReplaceGenerators(CreateGenerator(MIST));
			break;
		case 'C':
			system->ReplaceGenerators(CreateGenerator(DUST));
			break;
		case 'V':
			system->ReplaceGenerators(CreateGenerator(FIRE));
			break;
		case 'B':
			system->ReplaceGenerators(CreateGenerator(RAIN));
			break;
		case 'N':
			system->ReplaceGenerators(CreateGenerator(SPARK));
			break;
		case 'M':
			system->ReplaceGenerators(CreateGenerator(BLAST));
			break;
		case ',':
			system->ReplaceGenerators(CreateGenerator(SNOW));
			break;
		case '.':
			system->particles.Clear();
			break;
		default:
			break;
		}
	}
		break;
	case 4:
	{
		ParticleSystem* system = static_cast<ParticleSystem*>(particles.Get(0));

		switch (toupper(key)) {
		case 'Z':
			system->ShootFirework(BROCADE);
			break;
		case 'X':
			system->ShootFirework(CHRYSANTHEMUM);
			break;
		case 'C':
			system->ShootFirework(COMET);
			break;
		case 'V':
			system->ShootFirework(DELAY_CRACKLE);
			break;
		case 'B':
			system->ShootFirework(GORDO);
			break;
		case 'N':
			system->ShootFirework(FOUNTAIN_FIREWORK);
			break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
}

void Scene::ThrowProyectile(ProjectileType type, const physx::PxTransform& camera)
{
	const PxVec3 front = -camera.q.getBasisVector2().getNormalized();
	const PxVec3 up = -camera.q.getBasisVector0().getNormalized();

	//Crea el proyectil delante de la camara
	Particle* p = new Particle(camera.p + 1.5 * front);

	Projectile projectile = CreateProjectile(type, camera, p);
	
	//Se lanza hacia el frente con una varianza
	PxVec3 dir = PxVec3(PxCos(projectile.angle) * projectile.speed * front
		+ PxSin(projectile.angle) * projectile.speed * up);

	if (projectile.variation != 0) {
		normal_distribution<> normal(0, projectile.variation);
		dir = PxVec3(dir.x + normal(generator), dir.y + normal(generator), dir.z + normal(generator));
	}
	
	p->SetVel(dir);

	p->SetAcc({ 0, float(projectile.gravity), 0 });
	p->SetIMass(projectile.inverseMass);
	p->SetLifetime(10);

	particles.Add(p);
}
