#include "Particle.h"
#include "RenderUtils.hpp"

Particle::Particle(Vector3 pos, Vector3 v, Vector3 a, float d, float m) : pose(pos), vel(v), ac(a), damping(d), mass(m),
				force(Vector3(0, 0, 0)){
	pose = PxTransform(pos);
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1)), &pose, Vector4(1, 1, 1, 1));
}

Particle::~Particle() {
	DeregisterRenderItem(renderItem);
}
void Particle::integrate(double t) {
	//Movimiento basado en la 1a Ley de Newton

	// Calcula la aceleración con la fórmula: a = F / m
	Vector3 totalAcc = ac + (force / mass);

	// Actualizar la velocidad: v = v0 + a * t
	vel += totalAcc * t;

	// Aplicar damping (amortiguación)
	//vel *= pow(damping, t);
	vel = vel * damping + ac;
	pose.p = pose.p + vel * t;
}

//Practica Proyectiles
//Gsim = (Vsim * Vsim / Vreal * Vreal) * g real