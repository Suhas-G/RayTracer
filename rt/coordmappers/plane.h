#ifndef CG1RAYTRACER_COORDMAPPERS_PLANE_HEADER
#define CG1RAYTRACER_COORDMAPPERS_PLANE_HEADER

#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {

class Intersection;
class Vector;

class PlaneCoordMapper : public CoordMapper{
public:
    PlaneCoordMapper(const Vector& e1, const Vector& e2);
    virtual Point getCoords(const Intersection& hit) const;
private:
    Matrix transformation;
};

}

#endif