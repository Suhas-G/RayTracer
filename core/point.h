#ifndef CG1RAYTRACER_POINT_HEADER
#define CG1RAYTRACER_POINT_HEADER

#include <core/assert.h>
#include <core/macros.h>

namespace rt {

class Vector;
class HomogeneousCoord;

class ALIGN(16) Point {
public:
    float x, y, z;

    Point() = default;
    Point(float x, float y, float z);
    explicit Point(const HomogeneousCoord& coord);

    static Point rep(float v) { return Point(v,v,v); }

    Vector operator-(const Point& b) const;

    bool operator==(const Point& b) const;
    bool operator!=(const Point& b) const;
    float operator[](const int index) const;
    float& operator[](int index);

    explicit operator Vector() const;
    float distance() const;
    float distanceSqr() const;
    friend std::ostream& operator<<(std::ostream& os, const Point& p);

};

Point operator*(float scalar, const Point& b);
Point operator*(const Point& a, float scalar);

Point operator+(float scalar, const Point& b);
Point operator+(const Point& a, float scalar);

Point operator+(const Point& a, const Point& b);

Point operator-(float scalar, const Point& b);
Point operator-(const Point& a, float scalar);

Point min(const Point& a, const Point& b);
Point max(const Point& a, const Point& b);

Point min(const Point& a, const Point& b, const Point& c);
Point max(const Point& a, const Point& b, const Point& c);

}

#endif