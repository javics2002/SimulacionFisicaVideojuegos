#include "Scene.h"
#include <iostream>

#include "Particles/Firework.h"
#include "Particles/ParticleSystem/ParticleSystems.h"
#include "Particles/RigidParticle.h"
#include "Constraints/Rope.h"
#include "Constraints/Contact.h"
#include "Pinball/Pinball.h"

//DBG_NEW dice donde hay memory leaks
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

double Scene::AddToAllSprings(double value, bool k)
{
	/*Con este metodo he aprendido que dynamic_cast solo funciona 
	para coger una clase padre de una clase mas concreta. Pero si 
	intentas usarlo para comprobar que una clase general es de un 
	tipo concreto, da error de lectura. Juraria que lo habia usado 
	asi antes, y estaba convencidisimo de que podia usarlo para 
	saber si un forceGenerator era un muelle*/

	double totalVal = -1;
	for (auto forceParticle : fr) {
		if (forceParticle.first->type == SPRING) {
			Spring* spring = static_cast<Spring*>(forceParticle.first);
			totalVal = k ? spring->AddK(value) : spring->AddRestLength(value);
		}
	}
	for (auto forceParticle : frr) {
		if (forceParticle.first->type == SPRING_RIGID) {
			SpringRigid* spring = static_cast<SpringRigid*>(forceParticle.first);
			totalVal = k ? spring->AddK(value) : spring->AddRestLength(value);
		}
	}

	return totalVal;
}

void Scene::ShowSpringsValue(double value, bool k)
{
	if (value == -1) {
		cout << "Hemos perdido alguna bolita. Reseteando el sistema.\n"; //XD
		LoadScene(mID);
	}
	else
		cout << (k ? "k = " : "Longitud de reposo = ") << value << "\n";
}

PxRigidStatic* Scene::AddPxStatic(PxVec3 pos, PxShape* shape, PxVec4 color,
	PhysicMaterial material = DEFAULT) {
	shape->setMaterials(&gMaterials[material], 1);
	PxRigidStatic* particle = gPhysics->createRigidStatic(PxTransform(pos));
	particle->attachShape(*shape);
	renderItems.push_back(new RenderItem(shape, particle, color));
	gScene->addActor(*particle);
	return particle;
}

Scene::Scene() : particles(ParticleManager(&fr))
{
	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

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
		<< "': Goma elastica\n"
		<< "R: Particulas unidas mediante muelles\n"
		<< "T: Flotacion\n"
		<< "Y: Solidos rigidos\n"
		<< "U: Muelle realista\n"
		<< "I: Particulas unidas mediante muelles realistas\n"
		<< "O: Cuerda con colisiones\n"
		<< "P: Pinball\n\n";

	LoadScene(LAST_SCENE);
}

Scene::~Scene()
{
	ClearScene();

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
}

void Scene::LoadScene(int newID)
{
	ClearScene();
	glClearColor(0.3f, 0.4f, 0.5f, 1.0);

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
		glClearColor(0.0f, 0.0f, 0.05f, 1.0);
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
	{
		const double distance = 2;
		Particle* ball = (DBG_NEW Particle({ 40, 47, 40 }, true, true))->SetColor({ 1, 1, .3, 1 })
			->SetShape(CreateShape(PxSphereGeometry(.2)))->SetIntegrationMethod(SEMI_IMPLICIT_EULER)
			->SetDamp(.7);
		particles.Add(ball);
		Particle* ball2 = (DBG_NEW Particle(ball))->SetColor({ 1, .1, .4, 1.0 });
		ball2->Translate({ 0, 0, distance });
		particles.Add(ball2);

		fg.push_back(DBG_NEW RubberBand(ball2, 10, distance));
		fr.AddRegistry(fg[0], ball);
		fg.push_back(DBG_NEW RubberBand(ball, 10, distance));
		fr.AddRegistry(fg[1], ball2);
	}
		break;
	case 11:
	{
		const int numBolas = 5;
		const double distance = .7;

		Particle* lastBall = (DBG_NEW Particle({ 40, 52, 40 }, true, true))->SetColor({ 1, .1, .4, 1.0 })
			->SetShape(CreateShape(PxSphereGeometry(.2)))->SetIntegrationMethod(SEMI_IMPLICIT_EULER)
			->SetDamp(.7);
		particles.Add(lastBall);

		fg.push_back(DBG_NEW Gravity());
		fr.AddRegistry(fg[0], lastBall);
		fg.push_back(DBG_NEW AnchoredSpring(lastBall->GetPos() + PxVec3(0, distance, 0), 10, distance));
		fr.AddRegistry(fg[1], lastBall);

		for (int i = 1; i < numBolas; i++) {
			Particle* newBall = DBG_NEW Particle(lastBall);
			newBall->Translate({ 0, -distance, 0 });
			particles.Add(newBall);

			fr.AddRegistry(fg[0], newBall);
			fg.push_back(DBG_NEW Spring(lastBall, 10, distance));
			fr.AddRegistry(fg[2 * i], newBall);
			fg.push_back(DBG_NEW Spring(newBall, 10, distance));
			fr.AddRegistry(fg[2 * i + 1], lastBall);

			lastBall = newBall;
		}
	}
		break;
	case 12:
	{
		float l = .1;
		Particle* p = (DBG_NEW Particle({ 40, 50, 40 }, true, true))
			->SetShape(CreateShape(PxBoxGeometry(l, l, l)))->SetDamp(.7)
			->SetIMass(4);

		fg.push_back(DBG_NEW Gravity());
		fg.push_back(DBG_NEW Buoyancy({40, 45, 40}, l, 1000, pow(l, 3)));
		fr.AddRegistry(fg[0], p);
		fr.AddRegistry(fg[1], p);
		particles.Add(p);
	}
		break;
	case 13:
		AddPxStatic({ 0, 30, 0 }, CreateShape(PxBoxGeometry(100, 1, 100)), { .42, .23, .16, 1 }, SOAP);
		AddPxStatic({ -1, 40, -1 }, CreateShape(PxBoxGeometry(.5, 11, .5)), { .3, .3, .3, 1 });
		AddPxStatic({ 21, 31, 0 }, CreateShape(PxSphereGeometry(2)), { .9, .7, 1, 1 });
		AddPxStatic({ 25, 30.5, 0 }, CreateShape(PxBoxGeometry(2, 2, 10)), { .3, .3, .3, 1 });
		AddPxStatic({ 10, 35, -10 }, CreateShape(PxBoxGeometry(20, 5, 2)), { .9, .9, .9, 1 });
		AddPxStatic({ -30, 37, 0 }, CreateShape(PxBoxGeometry(20, 5, 10)), { .3, .3, 1, 1 });
		break;
	case 14:
	{
		Particle* ball = (DBG_NEW Particle({ 40, 48, 40 }, true, true))->SetColor({ 1, .1, .4, 1.0 })
			->SetShape(CreateShape(PxSphereGeometry(.2)))->SetIntegrationMethod(SEMI_IMPLICIT_EULER)
			->SetDamp(.7);
		particles.Add(ball);

		fg.push_back(DBG_NEW Gravity());
		fr.AddRegistry(fg[0], ball);
		fg.push_back(DBG_NEW AnchoredRealSpring({ 40, 50, 40 }, 10, 2, 1, 4.5, 10));
		fr.AddRegistry(fg[1], ball);

		//Marcas
		particles.Add((DBG_NEW Particle({ 40, 49, 39 }))->SetColor({ 0, 0, 0, 1.0 })
			->SetShape(CreateShape(PxBoxGeometry(.2, .05, .1))));
		particles.Add((DBG_NEW Particle({ 40, 45.5, 39 }))->SetColor({ 1, 1, 0, 1.0 })
			->SetShape(CreateShape(PxBoxGeometry(.2, .05, .1))));
		particles.Add((DBG_NEW Particle({ 40, 40, 39 }))->SetColor({ 1, 0, 0, 1.0 })
			->SetShape(CreateShape(PxBoxGeometry(.2, .05, .1))));
	}
		break;
	case 15:
	{
		const int numBolas = 5;
		const double distance = .7;
		const double minDistance = .5, maxDistance = 3, breakingDistance = 4;
		const double k = 50;

		Particle* lastBall = (DBG_NEW Particle({ 40, 52, 40 }, true, true))->SetColor({ 1, .1, .4, 1.0 })
			->SetShape(CreateShape(PxSphereGeometry(.2)))->SetIntegrationMethod(SEMI_IMPLICIT_EULER)
			->SetDamp(.7);
		particles.Add(lastBall);

		fg.push_back(DBG_NEW Gravity());
		fr.AddRegistry(fg[0], lastBall);
		fg.push_back(DBG_NEW AnchoredRealSpring(lastBall->GetPos() + PxVec3(0, distance, 0),
			k, distance, minDistance, maxDistance, breakingDistance));
		fr.AddRegistry(fg[1], lastBall);

		for (int i = 1; i < numBolas; i++) {
			Particle* newBall = DBG_NEW Particle(lastBall);
			newBall->Translate({ 0, -distance, 0 });
			particles.Add(newBall);

			fr.AddRegistry(fg[0], newBall);
			fg.push_back(DBG_NEW RealSpring(lastBall, k, distance, 
				minDistance, maxDistance, breakingDistance));
			fr.AddRegistry(fg[2 * i], newBall);
			fg.push_back(DBG_NEW RealSpring(newBall, k, distance, 
				minDistance, maxDistance, breakingDistance));
			fr.AddRegistry(fg[2 * i + 1], lastBall);

			lastBall = newBall;
		}
	}
		break;
	case 16:
	{
		const double radius = .2, distance = 2, restitution = .5;
		Particle* ball = (DBG_NEW Particle({ 40, 47, 40 }, true, true))->SetColor({ 1, 0, 0, 1 })
			->SetShape(CreateShape(PxSphereGeometry(radius)))->SetIntegrationMethod(SEMI_IMPLICIT_EULER)
			->SetDamp(.7);
		particles.Add(ball);
		Particle* ball2 = (DBG_NEW Particle(ball))->SetColor({ 0, 0, 1, 1.0 });
		ball2->Translate({ 0, 0, distance });
		particles.Add(ball2);

		links.push_back(new Rope(ball, ball2, distance * 2, restitution));
		contacts.push_back(new Contact(ball, radius, ball2, radius, restitution));
	}
		break;
	/*case 17:
		glClearColor(0.0f, 0.0f, 0.0f, 1.0);

		pinball = new Pinball();
		break;*/
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
			<< "B: Disminuir distancia del muelle\n"
			<< "N: Cambiar a Euler\n"
			<< "M: Cambiar a Euler implicito\n"
			<< ",: Crear tornado\n"
			<< ".: Resetear particula\n";
		break;
	case 10:
		cout << "Puedes interactuar con la bola amarilla.\n"
			<< "Z: Cambiar a Euler\n"
			<< "X: Cambiar a Euler implicito\n"
			<< "F: Aumentar k\n"
			<< "V: Disminuir k\n"
			<< "G: Aumentar distancia del muelle\n"
			<< "B: Disminuir distancia del muelle\n"
			<< "J: Empujarla al frente\n"
			<< "M: Empujarla atras\n"
			<< ",: Empujarla a la derecha\n"
			<< "N: Empujarla a la izquierda\n"
			<< "K: Empujarla arriba\n"
			<< "H: Empujarla abajo\n"
			<< ".: Resetear goma\n";
		break;
	case 11:
		cout << "Puedes interactuar con el muelle.\n"
			<< "Z: Tirar bolita\n"
			<< "X: Empujar bolita\n"
			<< "C: Tocar bolita\n"
			<< "F: Aumentar k\n"
			<< "V: Disminuir k\n"
			<< "G: Aumentar distancia del muelle\n"
			<< "B: Disminuir distancia del muelle\n"
			<< "N: Cambiar a Euler\n"
			<< "M: Cambiar a Euler implicito\n"
			<< ",: Crear tornado\n"
			<< ".: Resetear slinky\n";
		break;
	case 12:
		cout << "Puedes cambiar los valores del sistema.\n"
			<< "F: Aumentar densidad del liquido\n"
			<< "V: Disminuir densidad del liquido\n"
			<< "G: Aumentar masa del cubo\n"
			<< "B: Disminuir masa del cubo\n"
			<< "H: Aumentar volumen del cubo\n"
			<< "N: Disminuir volumen del cubo\n"
			<< ".: Resetear cubo\n";
		break;
	case 13:
		cout << "Puedes provocar una explosion con Z.\n";
		break;
	case 14:
		cout << "Puedes interactuar con el muelle.\n"
			<< "Z: Tirar bolita\n"
			<< "X: Empujar bolita\n"
			<< "C: Tocar bolita\n"
			<< "F: Aumentar k\n"
			<< "V: Disminuir k\n"
			<< "G: Aumentar distancia del muelle\n"
			<< "B: Disminuir distancia del muelle\n"
			<< "N: Cambiar a Euler\n"
			<< "M: Cambiar a Euler implicito\n"
			<< ",: Crear tornado\n"
			<< ".: Resetear particula\n";
		break;
	case 15:
		cout << "Puedes interactuar con el muelle.\n"
			<< "Z: Tirar bolita\n"
			<< "X: Empujar bolita\n"
			<< "C: Tocar bolita\n"
			<< "F: Aumentar k\n"
			<< "V: Disminuir k\n"
			<< "G: Aumentar distancia del muelle\n"
			<< "B: Disminuir distancia del muelle\n"
			<< "N: Cambiar a Euler\n"
			<< "M: Cambiar a Euler implicito\n"
			<< ",: Crear tornado\n"
			<< ".: Resetear slinky\n";
		break;
	case 16:
		cout << "Puedes interactuar con la bola roja.\n"
			<< "Z: Aumentar distancia de la cuerda\n"
			<< "X: Disminuir distancia de la cuerda\n"
			<< "C: Empujarla atras\n"
			<< "V: Empujarla al frente\n"
			<< "F: Empujarla a la izquierda\n"
			<< "G: Empujarla a la derecha\n"
			<< "H: Empujarla arriba\n"
			<< "J: Empujarla abajo\n"
			<< "B: Pararla\n"
			<< "N: Cambiar a Euler\n"
			<< "M: Cambiar a Euler implicito\n"
			<< ".: Resetear goma\n";
		break;
	case 17:
		cout << "Juega al pinball!\n"
			<< "C: Pala izquierda\n"
			<< "N: Pala derecha\n"
			<< "M: Lanzar bola\n"
			<< "L: Resetear juego\n";
		//Distintos materiales para bola?
	default:
		break;
	}

	cout << "\n";
}

void Scene::Update(double t)
{
	fr.Integrate(t);
	frr.Integrate(t);

	particles.Integrate(t);
	
	for (int i = 0; i < links.size(); i++)
		links[i]->Update(t);
	for (int i = 0; i < contacts.size(); i++)
		contacts[i]->Update(t);

	gScene->simulate(t);
	gScene->fetchResults(true);

	switch (mID) {
	case 13:
	{
		const double spawnTime = .2;
		const int maxParticles = 500;

		static double lastSpawn = 0;
		static int nParticles = 0;
		if (lastSpawn > spawnTime && nParticles < maxParticles) {
			float scale = (250 + rand() % 750) / 500.0f;
			PxVec3 size = PxVec3(.5, .2, .3) * scale;
			PhysicMaterial material = PhysicMaterial(rand() % LAST_PXMATERIAL);
			float tensorMultiplier = (rand() % 100) * .1;

			/*El color de las particulas esta definido el material
			su brillo por su tensor de inercia
			y su tamano por su masa*/
			RigidParticle* particle = new RigidParticle({ 0, 50, 0 }, pow(scale, 3),
				CreateShape(PxBoxGeometry(size)),
				colors[material] * tensorMultiplier * .1, material);

			particles.Add(particle);
			particle->particle->setMassSpaceInertiaTensor(
				PxVec3(size.y * size.z, size.x * size.z, size.x * size.y) * tensorMultiplier);
			particle->particle->setLinearVelocity({ 10, 2, 0 });
			nParticles++;
			lastSpawn = 0;
		}
		else
			lastSpawn += t;
	}
	break;
	default:
		break;
	}
}

void Scene::ClearScene()
{
	fr.clear();
	for (auto f : fg)
		delete f;
	fg.clear();

	particles.Clear();
	contacts.clear();
	links.clear();

	if (gScene != NULL) {
		gScene->release();

		for (auto item : renderItems) {
			DeregisterRenderItem(item);
			delete item;
		}

		renderItems.clear();
	}
	gScene = gPhysics->createScene(*sceneDesc);
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
		if (ball == nullptr) {
			cout << "La bolita se ha ido muy lejos. Se va a recargar la escena\n";
			LoadScene(mID);
			break;
		}

		switch (toupper(key)) {
		case 'Z': 
			Impulse(ball, { 0, -1000, 0 });
			break;
		case 'X': 
			Impulse(ball, { 0, 1000, 0 });
			break;
		case 'C':
			Impulse(ball, { 1000, 0, 0 });
			break;
		case 'F':
			ShowSpringsValue(AddToAllSprings(10, true), true);
			break;
		case 'V':
			ShowSpringsValue(AddToAllSprings(-10, true), true);
			break;
		case 'G':
			ShowSpringsValue(AddToAllSprings(.25, false), false);
			break;
		case 'B':
			ShowSpringsValue(AddToAllSprings(-.25, false), false);
			break;
		case 'N':
			ball->SetIntegrationMethod(EULER);
			cout << "Metodo de integraccion seleccionado: Euler\n";
			break;
		case 'M':
			ball->SetIntegrationMethod(SEMI_IMPLICIT_EULER);
			cout << "Metodo de integraccion seleccionado: Euler implicito\n";
			break;
		case ',':
			fr.AddRegistry(DBG_NEW Whirlwind(PxVec3(0, 30, 0), 100, 1), ball);
			break;
		case '.':
			LoadScene(mID);
			break;
		default:
			break;
		}
	}
		break;
	case 10:
	{
		vector<Particle*> balls;
		balls.push_back(particles.Get(0));
		balls.push_back(particles.Get(1));

		if (balls[0] == nullptr || balls[1] == nullptr) {
			cout << "Alguna bolita se ha ido muy lejos. Se va a recargar la escena\n";
			LoadScene(mID);
			break;
		}

		const double impulse = 100;

		switch (toupper(key)) {
		case 'Z':
			for(auto ball : balls)
				ball->SetIntegrationMethod(EULER);
			cout << "Metodo de integraccion seleccionado: Euler\n";
			break;
		case 'X':
			for (auto ball : balls)
				ball->SetIntegrationMethod(SEMI_IMPLICIT_EULER);
			cout << "Metodo de integraccion seleccionado: Euler implicito\n";
			break;
		case 'F':
			ShowSpringsValue(AddToAllSprings(10, true), true);
			break;
		case 'V':
			ShowSpringsValue(AddToAllSprings(-10, true), true);
			break;
		case 'G':
			ShowSpringsValue(AddToAllSprings(.25, false), false);
			break;
		case 'B':
			ShowSpringsValue(AddToAllSprings(-.25, false), false);
			break;
		case 'J':
			Impulse(balls[0], { 0, 0, -impulse });
			break;
		case 'M':
			Impulse(balls[0], { 0, 0, impulse });
			break;
		case ',':
			Impulse(balls[0], { impulse, 0, 0});
			break;
		case 'N':
			Impulse(balls[0], { -impulse, 0, 0 });
			break;
		case 'K':
			Impulse(balls[0], { 0, impulse, 0 });
			break;
		case 'H':
			Impulse(balls[0], { 0, -impulse, 0 });
			break;
		case '.':
			LoadScene(mID);
			break;
		default:
			break;
		}
	}
		break;
	case 11:
		switch (toupper(key)) {
		case 'Z':
			if(!particles.Empty())
				Impulse(particles.Get(particles.Size() - 1), { 0, -1000, 0 });
			break;
		case 'X':
			if (!particles.Empty())
				Impulse(particles.Get(particles.Size() - 1), { 0, 1000, 0 });
			break;
		case 'C': 
			if (!particles.Empty())
				Impulse(particles.Get(particles.Size() - 1), { 1000, 0, 0 });
			break;
		case 'F':
			ShowSpringsValue(AddToAllSprings(10, true), true);
			break;
		case 'V':
			ShowSpringsValue(AddToAllSprings(-10, true), true);
			break;
		case 'G':
			ShowSpringsValue(AddToAllSprings(.25, false), false);
			break;
		case 'B':
			ShowSpringsValue(AddToAllSprings(-.25, false), false);
			break;
		case 'N':
			for (int i = 0; i < particles.Size(); i++)
				particles.Get(i)->SetIntegrationMethod(EULER);
			cout << "Metodo de integraccion seleccionado: Euler\n";
			break;
		case 'M':
			for (int i = 0; i < particles.Size(); i++)
				particles.Get(i)->SetIntegrationMethod(SEMI_IMPLICIT_EULER);
			cout << "Metodo de integraccion seleccionado: Euler implicito\n";
			break;
		case ',': {
			Whirlwind* whirlwind = DBG_NEW Whirlwind(PxVec3(0, 30, 0), 100, 1);
			for (int i = 0; i < particles.Size(); i++)
				fr.AddRegistry(whirlwind, particles.Get(i));
		}
			break;
		case '.':
			LoadScene(mID);
			break;
		default:
			break;
		}
		break;
	case 12:
	{
		Particle* cubo = particles.Get(0);
		if (cubo == nullptr) {
			cout << "El se ha ido muy lejos. Se va a recargar la escena\n";
			LoadScene(mID);
			break;
		}

		static float l = .1;

		switch (toupper(key)) {
		case 'F':
			cout << "Densidad del liquido = " << static_cast<Buoyancy*>(fg[1])->AddDensity(10) << "\n";
			break;
		case 'V':
			cout << "Densidad del liquido = " << static_cast<Buoyancy*>(fg[1])->AddDensity(-10) << "\n";
			break;
		case 'G':
			cout << "Masa del cubo = " << cubo->AddMass(.25) << "\n";
			break;
		case 'B':
			cout << "Masa del cubo = " << cubo->AddMass(-.25) << "\n";
			break;
		case 'H': 
			l += .1;
			static_cast<Buoyancy*>(fg[1])->SetHeight(l)->SetVolume(pow(l, 3));
			cubo->SetShape(CreateShape(PxBoxGeometry(l, l, l)));
			cout << "Volumen del cubo = " << pow(l, 3) << "m^3\n";
			break;
		case 'N':
			l -= .1;
			if (l < .1)
				l = .1;
			static_cast<Buoyancy*>(fg[1])->SetHeight(l)->SetVolume(pow(l, 3));
			cout << "Volumen del cubo = " << pow(l, 3) << "m^3\n";
			break;
		case '.':
			cubo->SetPos({ 40, 50, 40 })->SetVel(PxVec3(0))->ClearForce();
			break;
		default:
			break;
		}
	}
		break;
	case 13:
		switch (toupper(key)) {
		case 'Z':
		{
			ExplosionRigid* explosion = new ExplosionRigid(PxVec3(20, 30, 0), 100, 1000, 5);
			
			for (int i = 0; i < particles.SizeRigid(); i++)
				frr.AddRegistry(explosion, particles.GetRigid(i));
		}
			break;
		default:
			break;
		}
		break;
	case 14:
	{
		const int ballID = 0;
		Particle* ball = particles.Get(ballID);
		if (ball == nullptr) {
			cout << "La bolita se ha ido muy lejos. Se va a recargar la escena\n";
			LoadScene(mID);
			break;
		}

		switch (toupper(key)) {
		case 'Z':
			Impulse(ball, { 0, -1000, 0 });
			break;
		case 'X':
			Impulse(ball, { 0, 1000, 0 });
			break;
		case 'C':
			Impulse(ball, { 1000, 0, 0 });
			break;
		case 'F':
			ShowSpringsValue(AddToAllSprings(10, true), true);
			break;
		case 'V':
			ShowSpringsValue(AddToAllSprings(-10, true), true);
			break;
		case 'G':
			ShowSpringsValue(AddToAllSprings(.25, false), false);
			break;
		case 'B':
			ShowSpringsValue(AddToAllSprings(-.25, false), false);
			break;
		case 'N':
			ball->SetIntegrationMethod(EULER);
			cout << "Metodo de integraccion seleccionado: Euler\n";
			break;
		case 'M':
			ball->SetIntegrationMethod(SEMI_IMPLICIT_EULER);
			cout << "Metodo de integraccion seleccionado: Euler implicito\n";
			break;
		case ',':
			fr.AddRegistry(DBG_NEW Whirlwind(PxVec3(0, 30, 0), 100, 1), ball);
			break;
		case '.':
			LoadScene(mID);
			break;
		default:
			break;
		}
	}
	case 15:
		switch (toupper(key)) {
		case 'Z':
			if (!particles.Empty())
				Impulse(particles.Get(particles.Size() - 1), { 0, -1000, 0 });
			break;
		case 'X':
			if (!particles.Empty())
				Impulse(particles.Get(particles.Size() - 1), { 0, 1000, 0 });
			break;
		case 'C':
			if (!particles.Empty())
				Impulse(particles.Get(particles.Size() - 1), { 1000, 0, 0 });
			break;
		case 'F':
			ShowSpringsValue(AddToAllSprings(10, true), true);
			break;
		case 'V':
			ShowSpringsValue(AddToAllSprings(-10, true), true);
			break;
		case 'G':
			ShowSpringsValue(AddToAllSprings(.25, false), false);
			break;
		case 'B':
			ShowSpringsValue(AddToAllSprings(-.25, false), false);
			break;
		case 'N':
			for (int i = 0; i < particles.Size(); i++)
				particles.Get(i)->SetIntegrationMethod(EULER);
			cout << "Metodo de integraccion seleccionado: Euler\n";
			break;
		case 'M':
			for (int i = 0; i < particles.Size(); i++)
				particles.Get(i)->SetIntegrationMethod(SEMI_IMPLICIT_EULER);
			cout << "Metodo de integraccion seleccionado: Euler implicito\n";
			break;
		case ',': {
			Whirlwind* whirlwind = DBG_NEW Whirlwind(PxVec3(0, 30, 0), 100, 1);
			for (int i = 0; i < particles.Size(); i++)
				fr.AddRegistry(whirlwind, particles.Get(i));
		}
				break;
		case '.':
			LoadScene(mID);
			break;
		default:
			break;
		}
		break;
	case 16:
	{
		vector<Particle*> balls;
		balls.push_back(particles.Get(0));
		balls.push_back(particles.Get(1));

		if (balls[0] == nullptr || balls[1] == nullptr) {
			cout << "Alguna bolita se ha ido muy lejos. Se va a recargar la escena\n";
			LoadScene(mID);
			break;
		}

		const double impulse = 100;

		switch (toupper(key)) {
		case 'Z':
			ShowSpringsValue(AddToAllSprings(.25, false), false);
			break;
		case 'X':
			ShowSpringsValue(AddToAllSprings(-.25, false), false);
			break;
		case 'V':
			Impulse(balls[0], { 0, 0, -impulse });
			break;
		case 'C':
			Impulse(balls[0], { 0, 0, impulse });
			break;
		case 'F':
			Impulse(balls[0], { -impulse , 0, 0 });
			break;
		case 'G':
			Impulse(balls[0], { impulse , 0, 0 });
			break;
		case 'H':
			Impulse(balls[0], { 0, impulse , 0 });
			break;
		case 'J':
			Impulse(balls[0], { 0, -impulse , 0 });
			break;
		case 'B':
			balls[0]->SetVel(PxVec3(0));
			break;
		case 'N':
			for (auto ball : balls)
				ball->SetIntegrationMethod(EULER);
			cout << "Metodo de integraccion seleccionado: Euler\n";
			break;
		case 'M':
			for (auto ball : balls)
				ball->SetIntegrationMethod(SEMI_IMPLICIT_EULER);
			cout << "Metodo de integraccion seleccionado: Euler implicito\n";
			break;
		case '.':
			LoadScene(mID);
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