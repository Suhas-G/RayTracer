#ifndef CG1RAYTRACER_COORDMAPPERS_ENVIRONMENT_HEADER
#define CG1RAYTRACER_COORDMAPPERS_ENVIRONMENT_HEADER

#include <core/vector.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {

class Vector;

class EnvironmentMapper : public CoordMapper {
public:
    EnvironmentMapper() {};
    EnvironmentMapper(float uscale, float vscale);
    virtual Point getCoords(const Intersection& hit) const;
private:
    float uscale = 1;
    float vscale = 1;
};

}

#endif