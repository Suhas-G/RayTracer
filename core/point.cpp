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

Point operator * (float scalar, const Point& b) {
    return Point(scalar * b.x, scalar * b.y, scalar * b.z);
}

Point operator * (const Point& a, float scalar) {
    return scalar * a;
}

Point min(const Point& a, const Point& b) {
    return Point(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

Point max(const Point& a, const Point& b) {
    return Point(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

std::ostream &operator<<(std::ostream &os, Point const &p) { 
    return os << "x: " << p.x << " y: " << p.y << " z: " << p.z;
}

}
