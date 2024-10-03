#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "Vector3D.h"

#include <iostream>

std::string display_text = "Ej2";


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

	//Ej1: Crear una esfera en el 0, 0, 0
	PxShape *shape = CreateShape(PxSphereGeometry(1)); //Creacion de la forma 
	PxTransform* tr = new PxTransform(Vector3(0, 0, 0)); //Posicion de la esfera
	Vector4 color(1, 1, 1, 1); //Color blanco y Alpha solido
	RenderItem *render = new RenderItem(shape, tr, color); //Renderizacion de la forma con su tr y color

	//Ej2: Crear ejes de coordenadas

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
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
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
	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case ' ':
	{
		break;
	}
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