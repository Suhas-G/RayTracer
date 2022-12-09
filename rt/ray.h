#ifndef CG1RAYTRACER_RAY_HEADER
#define CG1RAYTRACER_RAY_HEADER

#include <core/vector.h>
#include <core/matrix.h>
#include <core/point.h>

namespace rt {

class Ray {
public:
    Point o;
    Vector d;
    Vector invD;
    int dirIsNeg[3];

    Ray() {}
    Ray(const Point& o, const Vector& d);
    Point getPoint(float distance) const;
    std::pair<Ray, float> transform(const Matrix& m) const;
    float getDistance(const Point& p) const;
};

}

#endif