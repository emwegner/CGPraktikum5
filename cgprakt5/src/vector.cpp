#include "vector.h"

#include <assert.h>
#include <math.h>

Vector::Vector(float x, float y, float z) {

    this->X = x;
    this->Y = y;
    this->Z = z;
}

Vector::Vector() {

}


float Vector::dot(const Vector& v) const {
    return this->X * v.X + this->Y * v.Y + this->Z * v.Z;
}


Vector Vector::cross(const Vector& v) const {
    return Vector(this->Y * v.Z - this->Z * v.Y, this->Z * v.X - this->X * v.Z,
        this->X * v.Y - this->Y * v.X);
}


Vector Vector::operator+(const Vector& v) const {
    return Vector(this->X + v.X, this->Y + v.Y, this->Z + v.Z);
}

Vector Vector::operator-(const Vector& v) const {
    return Vector(this->X - v.X, this->Y - v.Y, this->Z - v.Z);
}

Vector Vector::operator*(float c) const {
    return Vector(this->X * c, this->Y * c, this->Z * c);
}

Vector Vector::operator-() const {
    return Vector(-this->X, -this->Y, -this->Z);
}

Vector& Vector::operator+=(const Vector& v) {
    this->X += v.X;
    this->Y += v.Y;
    this->Z += v.Z;
    return *this;
}

Vector& Vector::normalize() {
    float length =
        sqrt(this->X * this->X + this->Y * this->Y + this->Z * this->Z);
    this->X /= length;
    this->Y /= length;
    this->Z /= length;
    return *this;
}

float Vector::length() const {
    return sqrt(this->X * this->X + this->Y * this->Y + this->Z * this->Z);
}

float Vector::lengthSquared() const {
    return this->X * this->X + this->Y * this->Y + this->Z * this->Z;
}

Vector Vector::reflection(const Vector& normal) const {
    return (*this - normal * 2 * this->dot(normal));
}

bool Vector::triangleIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const {
    Vector area = (b - a).cross(c - a);
    area = area * (1.0f / area.length());
    float d1 = a.dot(area);

    s = (d1 - area.dot(*this)) / area.dot(d);

    if (s < 0) return false;

    Vector p = *this + d * s;

    float ABC = (b - a).cross(c - a).length() / 2;
    float ABP = (b - a).cross(p - a).length() / 2;
    float ACP = (c - a).cross(p - a).length() / 2;
    float BCP = (c - b).cross(p - b).length() / 2;

    return ABC + 10e-6 >= ABP + ACP + BCP;
}
