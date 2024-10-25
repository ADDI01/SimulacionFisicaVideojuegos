#pragma once

#include "RenderUtils.hpp"
#include "Vector3D.h"

using namespace physx;

class Particle {

public:
	Particle(Vector3 pos, Vector3 v, Vector3 a, float d, float m);
	~Particle();

	void integrate(double t);

private:
	Vector3 vel; //Particle's velocity
	Vector3 ac;	//Particle's acceleration
	float damping; //Represents friction force
	float mass; //Particle's mass
	Vector3 force; //Force applied to the Particle
	physx::PxTransform pose; //Particle's transform
	RenderItem* renderItem;
};
