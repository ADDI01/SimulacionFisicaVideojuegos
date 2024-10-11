#include "Particle.h"
#include "RenderUtils.hpp"

Particle::Particle(Vector3 pos, Vector3 v, Vector3 a, float d) : pose(pos), vel(v), ac(a), damping(d) {
	pose = PxTransform(pos);
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &pose, Vector4(1, 1, 1, 1));
}

Particle::~Particle() {
	DeregisterRenderItem(renderItem);
}
void Particle::integrate(double t) {
	//Movimiento basado en la 1a Ley de Newton
	vel = vel * damping + ac;
	pose.p = pose.p + vel * t;
}

//Gsim = (Vsim * Vsim / Vreal * Vreal) * g real