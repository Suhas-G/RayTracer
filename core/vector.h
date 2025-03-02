#ifndef CG1RAYTRACER_VECTOR_HEADER
#define CG1RAYTRACER_VECTOR_HEADER

#include <core/macros.h>
#include <core/assert.h>

namespace rt {

class HomogeneousCoord;
class Point;
class ALIGN(16) Vector {
public:
    float x, y, z;

    Vector() = default;
    Vector(float x, float y, float z);
    explicit Vector(const HomogeneousCoord& coord);

    static Vector rep(float v) { return Vector(v,v,v); }

    Vector operator + (const Vector& b) const;
    Vector operator - (const Vector& b) const;

    Vector operator - () const;

    Vector normalize() const;

    float lensqr() const;
    float length() const;

    bool operator == (const Vector& b) const;
    bool operator != (const Vector& b) const;
    float& operator[](int index);

    explicit operator Point() const;
    friend std::ostream& operator<<(std::ostream& os, const Vector& v);
};

Vector operator * (float scalar, const Vector& b);
Vector operator * (const Vector& a, float scalar);
Vector operator / (const Vector& a, float scalar);
Vector operator / (float scalar, const Vector& a);
Vector cross(const Vector& a, const Vector& b);
float dot(const Vector& a, const Vector& b);

Vector min(const Vector& a, const Vector& b);
Vector max(const Vector& a, const Vector& b);

Point operator + (const Point& a, const Vector& b);
Point operator + (const Vector& a, const Point& b);
Point operator - (const Point& a, const Vector& b);
// Point operator * (const HomogeneousCoord& scale, const Point& p);

}

#endif