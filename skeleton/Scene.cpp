#include "Scene.h"
#include "RenderUtils.hpp"
#include <iostream>

#include "Particles/Firework.h"
#include "Particles/ParticleSystem/ParticleSystems.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

Scene::Scene() : particles(ParticleManager(&fr))
{
	cout << "Pulsa las teclas para cambiar entre las escenas.\n"
		<< "0: Particula con velocidad constante\n"
		<< "1: Particula con aceleracion y damping\n"
		<< "2: Proyectiles\n"
		<< "3: Sistema de particulas\n"
		<< "4: Fuegos artificiales\n"
		<< "5: Fuerza gravitatoria\n"
		<< "6: Viento\n"
		<< "7: Torbellino\n"
		<< "8: Explosion\n"
		<< "9: Particula unida a una posicion estatica con un muelle\n"
		<< "': Particulas unidas mediante muelles\n"
		<< "Q: Flotacion\n\n";

	LoadScene(LAST_SCENE);
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
		particles.Add((DBG_NEW Particle())->SetVel({ 0, 10, 0 })->SetColor({ .98, .96, .3, 1.0 }));
		break;
	case 1:
		particles.Add((DBG_NEW Particle())->SetAcc({ 0, 2, 0 })->SetDamp(.9)->SetColor({ .31, .81, .96, 1.0 }));
		break;
	case 2:
		//Suelo
		particles.Add((DBG_NEW Particle({ 0, 48.5, 0 }))->SetColor({ .93, .81, .61, 1.0 })
			->SetShape(CreateShape(PxBoxGeometry(1000, .5, 1000))));

		//Vallas
		particles.Add((DBG_NEW Particle({ 50, 48.5, 40 }))->SetColor({ .2, .6, .9, 1.0 })
			->SetShape(CreateShape(PxBoxGeometry(10, 1, .5))));
		particles.Add((DBG_NEW Particle({ 40, 48.5, 50 }))->SetColor({ .2, .6, .9, 1.0 })
			->SetShape(CreateShape(PxBoxGeometry(.5, 1, 10))));

		//Diana
		particles.Add((DBG_NEW Particle({ 42, 50, 42 }))->SetColor({ .8, .5, .4, 1.0 })
			->SetShape(CreateShape(PxSphereGeometry(.6))));
		break;
	case 3:
		particles.Add(DBG_NEW ParticleSystem(&fr, CreateGenerator(FOUNTAIN)));
		break;
	case 4:
		particles.Add(DBG_NEW ParticleSystem());
		break;
	case 5:
	{
		//Particulas con aceleracion constante
		particles.Add((DBG_NEW Particle({ -10, 50, -10 }))->SetAcc({ 0, -9.8, 0 })->SetDamp(.9)->SetIMass(10)
			->SetColor({ 1, 0, 1, 1.0 })->SetShape(CreateShape(PxSphereGeometry(.5))));
		particles.Add((DBG_NEW Particle({ 0, 50, -10 }))->SetAcc({ 0, -9.8, 0 })->SetDamp(.9)->SetIMass(1)
			->SetColor({ 1, 1, 0, 1.0 })->SetShape(CreateShape(PxSphereGeometry(1))));
		particles.Add((DBG_NEW Particle({ 10, 50, -10 }))->SetAcc({ 0, -9.8, 0 })->SetDamp(.9)->SetIMass(.1)
			->SetColor({ 0, 1, 1, 1.0 })->SetShape(CreateShape(PxSphereGeometry(2))));

		//Particulas con gravedad como ForceGenerator
		Particle* p1, * p2, * p3;
		p1 = (DBG_NEW Particle({ -10, 50, 0 }, true, true))->SetDamp(.9)->SetIMass(10)
			->SetColor({ 1, 0, 0, 1.0 })->SetShape(CreateShape(PxSphereGeometry(.5)));
		p2 = (DBG_NEW Particle({ 0, 50, 0 }, true, true))->SetDamp(.9)->SetIMass(1)
			->SetColor({ 0, 1, 0, 1.0 })->SetShape(CreateShape(PxSphereGeometry(1)));
		p3 = (DBG_NEW Particle({ 10, 50, 0 }, true, true))->SetDamp(.9)->SetIMass(.1)
			->SetColor({ 0, 0, 1, 1.0 })->SetShape(CreateShape(PxSphereGeometry(2)));
		particles.Add(p1);
		particles.Add(p2);
		particles.Add(p3);

		fg.push_back(DBG_NEW Gravity());
		fr.AddRegistry(fg[0], p1);
		fr.AddRegistry(fg[0], p2);
		fr.AddRegistry(fg[0], p3);

		//Gravedad lunar
		Particle* p4,* p5,* p6;
		p4 = (DBG_NEW Particle({ -10, 50, 10 }, true, true))->SetIMass(10)
			->SetColor({ 1, .5, .5, 1.0 })->SetShape(CreateShape(PxSphereGeometry(.5)));
		p5 = (DBG_NEW Particle({ 0, 50, 10 }, true, true))->SetIMass(1)
			->SetColor({ .5, 1, .5, 1.0 })->SetShape(CreateShape(PxSphereGeometry(1)));
		p6 = (DBG_NEW Particle({ 10, 50, 10 }, true, true))->SetIMass(.1)
			->SetColor({ .5, .5, 1, 1.0 })->SetShape(CreateShape(PxSphereGeometry(2)));
		particles.Add(p4);
		particles.Add(p5);
		particles.Add(p6);

		fg.push_back(DBG_NEW Gravity({0, -1.62, 0}));
		fr.AddRegistry(fg[1], p4);
		fr.AddRegistry(fg[1], p5);
		fr.AddRegistry(fg[1], p6);
	}
		break;
	case 6:
	{
		//Como accedo a los datos que necesito en la tarea opcional?
		//El area que da al viento y la forma y el radio son solo shape para la particula
		shared_ptr<Wind> wind = std::make_shared<Wind>(PxVec3(1, 0, -3));
		ParticleSystem* system = DBG_NEW ParticleSystem(&fr,
			(DBG_NEW BoxParticleGenerator((DBG_NEW Particle({ 0, 0, 0 },
				false, true))->SetShape(CreateShape(PxSphereGeometry(.5)))
				->SetWindFriction1(1)->SetWindFriction2(0)->SetDamp(.6),
				1000, { 0, 50, 0 }, { 50, 50, 50 }, { .1,.1,.1 }, 1, &fr))
			->AddForceGenerator(wind));
		system->AddGenerator((DBG_NEW BoxParticleGenerator((DBG_NEW Particle({ 0, 0, 0 },
			false, true))->SetShape(CreateShape(PxSphereGeometry(.5)))->SetColor({ 1, .8, .3, 1 })
			->SetWindFriction1(0)->SetWindFriction2(.01)->SetDamp(.6),
			1000, { 0, 50, 0 }, { 50, 50, 50 }, { .1,.1,.1 }, 1, &fr))
			->AddForceGenerator(wind));
		system->AddGenerator((DBG_NEW BoxParticleGenerator((DBG_NEW Particle({ 0, 0, 0 },
			false, true))->SetShape(CreateShape(PxSphereGeometry(.5)))->SetColor({ .2, 1, .3, 1 })
			->SetWindFriction1(1)->SetWindFriction2(.01)->SetDamp(.6),
			1000, { 0, 50, 0 }, { 50, 50, 50 }, { .1,.1,.1 }, 1, &fr))
			->AddForceGenerator(wind));
		particles.Add(system);
	}
		break;
	case 7:
	{
		shared_ptr<Whirlwind> whirlwind 
			= std::make_shared<Whirlwind>(PxVec3(0, 30, 0), 100, 1);
		ParticleSystem* system = DBG_NEW ParticleSystem(&fr,
			(DBG_NEW BoxParticleGenerator((DBG_NEW Particle({ 0, 0, 0 },
				false, true))->SetWindFriction1(.5)->SetWindFriction2(0)->SetDamp(.6),
				3000, { 0, 30, 0 }, { 50, 40, 50 }, PxVec3(DBL_EPSILON), 1, &fr))
			->AddForceGenerator(whirlwind), { -50, 0, -50 });
		particles.Add(system);
	}
	break;
	case 8:
	{
		shared_ptr<Explosion> explosion 
			= std::make_shared<Explosion>(PxVec3(0, 30, 0), 100, 1000, 5000);
		ParticleSystem* system = DBG_NEW ParticleSystem(&fr,
			(DBG_NEW BoxParticleGenerator((DBG_NEW Particle({ -50, 0, -50 },
				false, true))->SetWindFriction1(10)->SetWindFriction2(0),
				3000, { 10, 30, 10 }, { 50, 40, 50 }, { .1,.1,.1 }, 1, &fr))
			->AddForceGenerator(explosion), { -20, 0, -20 });
		particles.Add(system);
	}
		break;
	case 9:
	{
		Particle* ball = (DBG_NEW Particle({ 40, 48, 40 }, true, true))->SetColor({ 1, .1, .4, 1.0 })
			->SetShape(CreateShape(PxSphereGeometry(.2)))->SetIntegrationMethod(SEMI_IMPLICIT_EULER)
			->SetDamp(.7);
		particles.Add(ball);

		fg.push_back(DBG_NEW Gravity());
		fr.AddRegistry(fg[0], ball);
		fg.push_back(DBG_NEW AnchoredSpring({ 40, 50, 40 }, 10, 2));
		fr.AddRegistry(fg[1], ball);
	}
		break;
	case 10:
		break;
	case 11:
		break;
	default:
		cout << "Scene " << mID << " doesn't exist.\n";
		return;
	}

	//Mensaje
	cout << "Scene " << mID << " loaded.\n";
	switch (mID) {
	case 2:
		cout << "Puedes disparar proyectiles.\n"
			<< "Z: Bala\n"
			<< "X: Artilleria\n"
			<< "C: Bola de fuego\n"
			<< "V: Laser\n"
			<< "B: Pelota de golf\n"
			<< "N: Frisbee\n"
			<< "M: Globo\n";
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
		break;
	case 4:
		cout << "Puedes lanzar fuegos artificiales.\n"
			<< "Z: Brocado\n"
			<< "X: Crisantemo\n"
			<< "C: Cometa\n"
			<< "V: Delay\n"
			<< "B: El gordo\n"
			<< "N: Fuente\n";
		break;
	case 9:
		cout << "Puedes interactuar con el muelle.\n"
			<< "Z: Tirar bolita\n"
			<< "X: Empujar bolita\n"
			<< "C: Tocar bolita\n"
			<< "F: Aumentar k\n"
			<< "V: Disminuir k\n"
			<< "G: Aumentar distancia del muelle\n"
			<< "V: Disminuir distancia del muelle\n"
			<< "N: Cambiar a Euler\n"
			<< "M: Cambiar a Euler implicito\n"
			<< ".: Resetear particula\n";
		break;
	default:
		break;
	}
}

void Scene::Update(double t)
{
	fr.Integrate(t);
	particles.Integrate(t);
}

void Scene::ClearScene()
{
	fr.clear();
	for (auto f : fg)
		delete f;
	fg.clear();

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
	case 9:
	{
		const int ballID = 0;
		Particle* ball = particles.Get(ballID);
		auto it = fr.find(fg[1]);
		while (ball == nullptr || it == fr.end()) {
			ball = (DBG_NEW Particle({ 40, 48, 40 }, true, true))->SetColor({ 1, .1, .4, 1.0 })
				->SetShape(CreateShape(PxSphereGeometry(.2)))->SetIntegrationMethod(SEMI_IMPLICIT_EULER)
				->SetDamp(.7);
			particles.Add(ball);

			fg.push_back(DBG_NEW Gravity());
			fr.AddRegistry(fg[0], ball);
			fg.push_back(DBG_NEW AnchoredSpring({ 40, 50, 40 }, 10, 1));
			it = fr.AddRegistry(fg[1], ball);
			cout << "La pelota se ha ido demasiado lejos. Se ha creado otra igual que la anterior.\n";
		}
		AnchoredSpring* spring = static_cast<AnchoredSpring*>(it->first);

		switch (toupper(key)) {
		case 'Z': {
			Impulse i = Impulse(ball, { 0, -1000, 0 });
		}
			break;
		case 'X': {
			Impulse i = Impulse(ball, { 0, 1000, 0 });
		}
				break;
		case 'C': {
			Impulse i = Impulse(ball, { 1000, 0, 0 });
		}
			break;
		case 'F':
			cout << "k = " << spring->AddK(10) << "\n";
			break;
		case 'V':
			cout << "k = " << spring->AddK(-10) << "\n";
			break;
		case 'G':
			cout << "Longitud de reposo = " << spring->AddRestLength(.25) << "\n";
			break;
		case 'B':
			cout << "Longitud de reposo = " << spring->AddRestLength(-.25) << "\n";
			break;
		case 'N':
			ball->SetIntegrationMethod(EULER);
			cout << "Metodo de integraccion seleccionado: Euler\n";
			break;
		case 'M':
			ball->SetIntegrationMethod(SEMI_IMPLICIT_EULER);
			cout << "Metodo de integraccion seleccionado: Euler implicito\n";
			break;
		case '.':
			particles.Remove(ballID);
			ball = (DBG_NEW Particle({ 40, 48, 40 }, true, true))->SetColor({ 1, .1, .4, 1.0 })
				->SetShape(CreateShape(PxSphereGeometry(.2)))->SetIntegrationMethod(SEMI_IMPLICIT_EULER)
				->SetDamp(.7);
			particles.Add(ball);

			fg.push_back(DBG_NEW Gravity());
			fr.AddRegistry(fg[0], ball);
			fg.push_back(DBG_NEW AnchoredSpring({ 40, 50, 40 }, 10, 1));
			it = fr.AddRegistry(fg[1], ball);
			cout << "Se ha reseteado la bolita.\n";
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
	Particle* p = DBG_NEW Particle(camera.p + 1.5 * front);

	const Projectile projectile = CreateProjectile(type, camera, p);
	
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
