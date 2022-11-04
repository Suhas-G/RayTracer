#include <core/scalar.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/homogeneouscoord.h>
#include <core/assert.h>
#include <algorithm>
#include <cmath>

namespace rt {

Vector::Vector(float x, float y, float z): x(x), y(y), z(z) {}

// Vector::Vector(const HomogeneousCoord& coord): x(coord[0] / coord[3]), y(coord[1] / coord[3]), z(coord[2] / coord[3]) {}

Vector::Vector(const HomogeneousCoord& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

Vector Vector::operator + (const Vector& b) const {
    Vector vec(x + b.x, y + b.y, z + b.z);
    return vec;
}

Vector Vector::operator - (const Vector& b) const {
    Vector vec(x - b.x, y - b.y, z - b.z);
    return vec;
}

Vector Vector::operator - () const {
    Vector vec(-x, -y, -z);
    return vec;
}

Vector Vector::normalize() const {
    float len = length();
    Vector vec(x / len, y / len, z / len);
    return vec;
}

Vector operator * (float scalar, const Vector& b) {
    Vector vec(scalar * b.x, scalar * b.y, scalar * b.z);
    return vec;
}

Vector operator * (const Vector& a, float scalar) {
    return scalar * a;
}

Vector operator / (const Vector& a, float scalar) {
    Vector vec(a.x / scalar, a.y / scalar, a.z / scalar);
    return vec;
}

Vector cross(const Vector& a, const Vector& b) {
    Vector vec((a.y * b.z)-(a.z * b.y), (a.z * b.x)-(a.x * b.z), (a.x * b.y)-(a.y * b.x));
    return vec;
}

float dot(const Vector& a, const Vector& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Vector::lensqr() const {
    return rt::dot(*this, *this);
}

float Vector::length() const {
    return std::sqrt(lensqr());
}


bool Vector::operator == (const Vector& b) const {
    return (
        std::abs(x - b.x) < rt::epsilon &&
        std::abs(y - b.y) < rt::epsilon &&
        std::abs(z - b.z) < rt::epsilon
    );
}

bool Vector::operator != (const Vector& b) const {
    return !(*this == b);
}

Vector min(const Vector& a, const Vector& b) {
    Vector vec(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
    return vec;
}

Vector max(const Vector& a, const Vector& b) {
    Vector vec(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
    return vec;
}

Point operator + (const Point& a, const Vector& b) {
   Point pt(a.x + b.x, a.y + b.y, a.z + b.z);
   return pt;
}

Point operator + (const Vector& a, const Point& b) {
    return b + a;
}

Point operator - (const Point& a, const Vector& b) {
    Point pt(a.x - b.x, a.y - b.y, a.z - b.z);
    return pt;
}

Point operator * (const HomogeneousCoord& scale, const Point& p) {
    // Point pt;
    // pt = Point((scale[0] * p.x), scale[1] * p.y, scale[2] * p.z) * (1/scale[3]);
    // return pt;
    /* TODO */ NOT_IMPLEMENTED;
}

}
