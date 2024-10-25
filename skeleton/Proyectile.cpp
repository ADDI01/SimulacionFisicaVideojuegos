#include "Proyectile.h"

Projectile::Projectile(Vector3 position, Vector3 direction, float rV, float sV, Vector3 acceleration, float mass, float rG,
    float damping, PxShape* shape)
    : Particle(position, direction * simV, acceleration, damping, mass) , realG(rG), p(nullptr), realV(rV), simV(sV) {

    calculateSimulatedGravity();
}

void Projectile::calculateSimulatedGravity()
{
    // Using the formula: G_sim = (V_sim^2 / V_real^2) * g_real
    simG = (simV * simV / (realV * realV)) * realG;
}

// Sobreescribimos la función integrate para incluir la gravedad específica del proyectil
void Projectile::integrate(double t) {
    if (t <= 0) return;

    // Agregar gravedad a la aceleración en el eje Y
    Vector3 gravityForce(0, -simG * mass, 0);
    addForce(gravityForce);

    Particle::integrate(t); // Llamada al método base que incluye el damping y actualización de posición
}

