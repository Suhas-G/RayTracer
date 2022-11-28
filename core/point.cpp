#include <core/point.h>
#include <core/homogeneouscoord.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <core/vector.h>

namespace rt {

Point::Point(float x, float y, float z): x(x), y(y), z(z) {}

Point::Point(const HomogeneousCoord& coord)
{
    /* TODO */ NOT_IMPLEMENTED;
}

Vector Point::operator - (const Point& b) const {
    Vector vec(x - b.x, y - b.y, z - b.z);
    return vec;
}

bool Point::operator == (const Point& b) const {
    return (
        std::abs(x - b.x) < rt::epsilon && 
        std::abs(y - b.y) < rt::epsilon &&
        std::abs(z - b.z) < rt::epsilon);
}

bool Point::operator != (const Point& b) const {
    return !(*this == b);
}

float Point::distance() const {
    return std::sqrt(distanceSqr());
}

float Point::distanceSqr() const {
    return x*x + y*y + z*z;
}

Point::operator Vector() const {
    return Vector(this->x, this->y, this->z);
}

float Point::operator[](const int index) const {
    if (index == 0)
        return this->x;
    if (index == 1)
        return this->y;
    if (index == 2)
        return this->z;
    throw rt::Assert() << "Out of bounds for 3 value point";
}

float& Point::operator[](int index) {
    if (index == 0)
        return this->x;
    if (index == 1)
        return this->y;
    if (index == 2)
        return this->z;
    throw rt::Assert() << "Out of bounds for 3 value point";
}

Point operator * (float scalar, const Point& b) {
    return Point(scalar * b.x, scalar * b.y, scalar * b.z);
}

Point operator * (const Point& a, float scalar) {
    return scalar * a;
}

Point operator+(const Point& a, float scalar) {
    return Point(a.x + scalar, a.y + scalar, a.z + scalar);
}
Point operator+(float scalar, const Point& b) {
    return b + scalar;
}

Point operator-(float scalar, const Point& b) {
    return Point(scalar - b.x, scalar - b.y, scalar - b.z);
}
Point operator-(const Point& a, float scalar) {
    return Point(a.x - scalar, a.y - scalar, a.z - scalar);
}

Point operator+(const Point& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point min(const Point& a, const Point& b) {
    return Point(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

Point min(const Point& a, const Point& b, const Point& c) {
    return Point(rt::min(a.x, b.x, c.x), rt::min(a.y, b.y, c.y), rt::min(a.z, b.z, c.z));
}

Point max(const Point& a, const Point& b) {
    return Point(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

Point max(const Point& a, const Point& b, const Point& c) {
    return Point(rt::max(a.x, b.x, c.x), rt::max(a.y, b.y, c.y), rt::max(a.z, b.z, c.z));
}

std::ostream &operator<<(std::ostream &os, Point const &p) { 
    return os << "Point(x: " << p.x << " y: " << p.y << " z: " << p.z << ")";
}

}
