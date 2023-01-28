#ifndef CG1RAYTRACER_NORMALMAPPERS_NORMALMAPPER_HEADER
#define CG1RAYTRACER_NORMALMAPPERS_NORMALMAPPER_HEADER

#include <core/vector.h>
#include <rt/intersection.h>

namespace rt {

class NormalMapper {
public:
    Vector getNormal(const Intersection& hit, const Point& coords);
    virtual Vector computePerturbation(const Intersection& hit, const Point& coords) const = 0;
    virtual ~NormalMapper() {}
};

}

#endif