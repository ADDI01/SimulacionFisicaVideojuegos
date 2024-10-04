#include "Particle.h"
#include "RenderUtils.hpp"

Particle::Particle(Vector3 pos, Vector3 v/*, Vector3 a*/) : pose(pos), vel(v) {
	pose = PxTransform(pos);
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &pose, Vector4(1, 1, 1, 1));
}

Particle::~Particle() {
	//delete renderItem;
	DeregisterRenderItem(renderItem);
}
void Particle::integrate(double t) {
	//Movimiento basado en la 1a Ley de Newton
	pose.p = pose.p + vel * t;
}