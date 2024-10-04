#pragma once

#include "RenderUtils.hpp"
#include "Vector3D.h"

using namespace physx;

class Particle {

public:
	Particle(Vector3 pos, Vector3 v/*, Vector3 a*/);
	~Particle();

	void integrate(double t);

private:
	Vector3 vel; //Particle's velocity
	Vector3 ac;	//Particle's acceleration
	physx::PxTransform pose; //Particle's transform
	RenderItem* renderItem;
};
