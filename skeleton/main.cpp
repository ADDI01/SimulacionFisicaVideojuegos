#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "Vector3D.h"
#include "Particle.h"
#include "Proyectile.h"

#include <iostream>

std::string display_text = "Ej3: R(X) V(Y) A(Z)";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

Particle* partX, *partY, *partZ;

// Vector para almacenar proyectiles
std::vector<Projectile*> projectiles;
const float CANNON_PROJ_SIM_SPEED = 7.0f;
const float CANNON_PROJ_REAL_SPEED = 250.f;

const float TANK_PROJ_SIM_SPEED = 16.0f;
const float TANK_PROJ_REAL_SPEED = 1800.0f;

const float LASERGUN_PROJ_SIM_SPEED = 25.0f;
const float LASERGUN_PROJ_REAL_SPEED = 300000000.0f;

const float g = 9.8f;


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	/*--------------------------------------------PRACTICA 0----------------------------------------------*/
	//Crear una esfera en el 0, 0, 0
	//PxShape *shape = CreateShape(PxSphereGeometry(1)); //Creacion de la forma 
	//PxTransform* tr = new PxTransform(Vector3(0, 0, 0)); //Posicion de la esfera
	//Vector4 color(1, 1, 1, 1); //Color blanco y Alpha solido
	//RenderItem *render = new RenderItem(shape, tr, color); //Renderizacion de la forma con su tr y color

	//Crear ejes de coordenadas

	//Creamos las formas que representan los ejes
	PxShape *sphereX = CreateShape(PxSphereGeometry(1));
	PxShape *sphereY = CreateShape(PxSphereGeometry(1));
	PxShape *sphereZ = CreateShape(PxSphereGeometry(1));

	//Definimos los tr de cada forma
	int escalar = 10;
	PxTransform* xTr = new PxTransform(1 * escalar, 0, 0);
	PxTransform* yTr = new PxTransform(0, 1 * escalar, 0);
	PxTransform* zTr = new PxTransform(0, 0, 1 * escalar);

	//Definimos los colores para diferenciar los ejes
	Vector4 colorX(1, 0, 0, 1);
	Vector4 colorY(0, 1, 0, 1);
	Vector4 colorZ(0, 0, 1, 1);

	//Renderizamos los objetos
	RenderItem* axisX = new RenderItem(sphereX, xTr, colorX);
	RenderItem* axisY = new RenderItem(sphereY, yTr, colorY);
	RenderItem* axisZ = new RenderItem(sphereZ, zTr, colorZ);

	/*--------------------------------------------PRACTICA 1: parte 1----------------------------------------------*/
	//Crear particula con vel cte
	/*Vector3 p(0, 0, 0), vX(10, 0, 0), vY(0, 10, 0), vZ(0, 0, 10);*/
	/*partX = new Particle(p, vX);
	partY = new Particle(p, vY);
	partZ = new Particle(p, vZ);*/

	//Crear particulas con aceleracion a
	/*Vector3 aX(10, 0, 0), aY(0, 15, 0), aZ(0, 0, 5);*/
	/*partX = new Particle(p, vX, aX);
	partY = new Particle(p, vY, aY);
	partZ = new Particle(p, vZ, aZ);*/

	//Crear particulas con damping y masa
	/*float d = 0.4f, m = 10.0f;
	partX = new Particle(p, vX, aX, d, m);
	partY = new Particle(p, vY, aY, d, m);
	partZ = new Particle(p, vZ, aZ, d, m);*/
}

/*--------------------------------------------PRACTICA 1: parte 2----------------------------------------------*/
// Crear proyectiles y añadirlos al vector
void shootProjectile(Vector3 position, Vector3 direction, float realSpeed, float simSpeed, float mass, float gravity, 
		float damping, PxShape* shape) {

	Vector3 ac(0, 0, 0);  // Inicialmente sin aceleración adicional
	Projectile* proj = new Projectile(position, direction, realSpeed, simSpeed, ac, mass, gravity, damping, shape);
	projectiles.push_back(proj);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	//partX->integrate(t); //Llamamos a la funcion de integracion de la particula
	//partY->integrate(t); //Llamamos a la funcion de integracion de la particula
	//partZ->integrate(t); //Llamamos a la funcion de integracion de la particula

	// Integrar todos los proyectiles activos
	for (auto& proj : projectiles) {
		proj->integrate(t);
	}
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	//delete partX; //Llamamos a la destrcutora de Particle para deregistrarla
	//delete partY; //Llamamos a la destrcutora de Particle para deregistrarla
	//delete partZ; //Llamamos a la destrcutora de Particle para deregistrarla

	for (auto& proj : projectiles) {
		delete proj;
	}
	projectiles.clear();

	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	//PX_UNUSED(camera);

	Vector3 camPosition(camera.p.x, camera.p.y, camera.p.z);
	Vector3 camDirection(camera.q.getBasisVector0().x, camera.q.getBasisVector0().y, camera.q.getBasisVector0().z);

	//Practica Proyectiles
	//Gsim = (Vsim * Vsim / Vreal * Vreal) * g real
	//Reducir la velocidad de disparo para que esté en el rango de 5 a 25 m / s
	switch(toupper(key))
	{
	case '1':  //Bala de cañon: Velocidad 7 m/s
		shootProjectile(camPosition, camDirection, CANNON_PROJ_SIM_SPEED, CANNON_PROJ_REAL_SPEED, 1.0f, g, 
			0.99f, CreateShape(PxSphereGeometry(0.5f)));
		break;
	case '2':  //Bala de tanque: Velocidad 16 m/s
		shootProjectile(camPosition, camDirection, TANK_PROJ_SIM_SPEED, TANK_PROJ_REAL_SPEED, 5.0f, g,
			0.98f, CreateShape(PxSphereGeometry(1.0f)));
		break;
	case '3':  //Pistola láser: Velocidad 25 m/s
		shootProjectile(camPosition, camDirection, LASERGUN_PROJ_SIM_SPEED, LASERGUN_PROJ_REAL_SPEED, 0.2f, g,
			0.95f, CreateShape(PxSphereGeometry(0.3f)));
		break;
	default:
		break;
	}
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