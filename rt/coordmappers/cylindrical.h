#ifndef CG1RAYTRACER_COORDMAPPERS_CYLINDRICAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_CYLINDRICAL_HEADER

#include <core/matrix.h>
#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {

class Vector;

class CylindricalCoordMapper : public CoordMapper {
public:
    CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis);
    virtual Point getCoords(const Intersection& hit) const;
private:
    Point origin;
    float yscale;
    float xscale;
    Vector xAxis;
    Vector yAxis;
    Vector zAxis;
};

}

#endif