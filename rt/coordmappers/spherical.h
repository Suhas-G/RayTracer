#ifndef CG1RAYTRACER_COORDMAPPERS_SPHERICAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_SPHERICAL_HEADER

#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {

class Vector;

class SphericalCoordMapper : public CoordMapper {
public:
    SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef);
    virtual Point getCoords(const Intersection& hit) const;
private:
    Point origin;
    Matrix transformation;
    float uscale;
    float vscale;
};

}

#endif