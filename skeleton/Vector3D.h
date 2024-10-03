#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <cmath>

class Vector3D {
private:
    float x, y, z;

public:
    // Constructores
    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vector3D(const Vector3D& other) : x(other.x), y(other.y), z(other.z) {}

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }

    // Setters
    void setX(float x_) { x = x_; }
    void setY(float y_) { y = y_; }
    void setZ(float z_) { z = z_; }

    // Operaciones con vectores

    //Suma de cada componente
    Vector3D operator+(const Vector3D& v) const {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }

    //Resta de cada componente
    Vector3D operator-(const Vector3D& v) const {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }

    //Multiplicacion por un escalar dado
    Vector3D operator*(float scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    //Division por un escalar dado
    Vector3D operator/(float scalar) const {
        if (scalar != 0)
            return Vector3D(x / scalar, y / scalar, z / scalar);
        throw std::runtime_error("Division by zero!");
    }

    // Producto escalar (dot product)
    float dot(const Vector3D& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    // Producto vectorial (cross product)
    Vector3D cross(const Vector3D& v) const {
        return Vector3D(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    // Magnitud (norma) del vector
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Normalización del vector
    Vector3D normalize() const {
        float mag = magnitude();
        if (mag > 0)
            return *this / mag;
        throw std::runtime_error("Cannot normalize a zero-length vector.");
    }

    // Operadores de comparación
    bool operator==(const Vector3D& v) const {
        return x == v.x && y == v.y && z == v.z;
    }

    bool operator!=(const Vector3D& v) const {
        return !(*this == v);
    }

    // Operador de acceso a los componentes
    float& operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        throw std::out_of_range("Index out of range");
    }

    const float& operator[](int index) const {
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        throw std::out_of_range("Index out of range");
    }

    // Operador de asignación
    Vector3D& operator=(const Vector3D& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

    // Sobrecarga de operador << para imprimir el vector
    friend std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
};

#endif