#pragma once

#include "Particle.h"
#include "Vector3D.h"

class Projectile : public Particle {
public:
    Projectile(Vector3 position, Vector3 direction, float rV, float sV, Vector3 acceleration, float mass, float rG,
        float damping, PxShape* shape);
    void calculateSimulatedGravity();  // Method to calculate G_sim
    void integrate(double t) override;

private:
    float realG, simG;
    float realV, simV; //Real and simulated velocity
    Particle* p; //Pointer to particle
};


