#include <core/homogeneouscoord.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/assert.h>
#include <core/scalar.h>

namespace rt {

HomogeneousCoord::HomogeneousCoord(float x, float y, float z, float w) {
    coords[0] = x;
    coords[1] = y;
    coords[2] = z;
    coords[3] = w;
}

HomogeneousCoord::HomogeneousCoord(const Point& p) {
    coords[0] = p.x;
    coords[1] = p.y;
    coords[2] = p.z;
    coords[3] = 1;
}

HomogeneousCoord::HomogeneousCoord(const Vector& vec) {
    coords[0] = vec.x;
    coords[1] = vec.y;
    coords[2] = vec.z;
    coords[3] = 0;
}

float& HomogeneousCoord::operator [] (int idx) {
    return coords[idx];
}

float HomogeneousCoord::operator [] (int idx) const {
    return coords[idx];
}

HomogeneousCoord HomogeneousCoord::operator + (const HomogeneousCoord& b) const {
    return HomogeneousCoord(coords[0] + b[0], coords[1] + b[1], coords[2] + b[2], coords[3] + b[3]);
}

HomogeneousCoord HomogeneousCoord::operator - (const HomogeneousCoord& b) const {
    return HomogeneousCoord(coords[0] - b[0], coords[1] - b[1], coords[2] - b[2], coords[3] - b[3]);
}

HomogeneousCoord HomogeneousCoord::operator * (const HomogeneousCoord& b) const {
    return HomogeneousCoord(coords[0] * b[0], coords[1] * b[1], coords[2] * b[2], coords[3] * b[3]);
}

HomogeneousCoord HomogeneousCoord::operator / (const HomogeneousCoord& b) const {
    return HomogeneousCoord(coords[0] / b[0], coords[1] / b[1], coords[2] / b[2], coords[3] / b[3]);
}

HomogeneousCoord operator * (float scalar, const HomogeneousCoord& b) {
    return HomogeneousCoord(scalar * b[0], scalar * b[1], scalar * b[2], scalar * b[3]);
}

HomogeneousCoord operator * (const HomogeneousCoord& a, float scalar) {
    return scalar * a;
}

HomogeneousCoord operator / (const HomogeneousCoord& a, float scalar) {
    return HomogeneousCoord(a[0] / scalar, a[1] / scalar, a[2] / scalar, a[3] / scalar);
}

float dot(const HomogeneousCoord& a, const HomogeneousCoord& b) {
    return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]) + (a[3] * b[3]);
}

float dot(const HomogeneousCoord& a, const Vector& b) {
    // Vector in Homogenous coord has w = 0
    return (a[0] * b.x) + (a[1] * b.y) + (a[2] * b.z);
}

float dot(const HomogeneousCoord& a, const Point& b) {
    // Point in Homogenous coord has w = 1
    return (a[0] * b.x) + (a[1] * b.y) + (a[2] * b.z) + (a[3] * 1.0f);
}

HomogeneousCoord HomogeneousCoord::operator - () const {
    return (-1 * (*this));
}

bool HomogeneousCoord::operator == (const HomogeneousCoord& b) const {
    return coords[0] == b[0] && coords[1] == b[1] && coords[2] == b[2] && coords[3] == b[3];
}

bool HomogeneousCoord::operator != (const HomogeneousCoord& b) const {
    return !(*this == b);
}

HomogeneousCoord min(const HomogeneousCoord& a, const HomogeneousCoord& b) {
    return HomogeneousCoord(std::min(a[0], b[0]), std::min(a[1], b[1]), std::min(a[2], b[2]), std::min(a[3], b[3]));
}

HomogeneousCoord max(const HomogeneousCoord& a, const HomogeneousCoord& b) {
    return HomogeneousCoord(std::max(a[0], b[0]), std::max(a[1], b[1]), std::max(a[2], b[2]), std::max(a[3], b[3]));
}

}