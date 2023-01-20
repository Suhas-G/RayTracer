#ifndef CG1RAYTRACER_SOLIDS_ENVIRONMENT_HEADER
#define CG1RAYTRACER_SOLIDS_ENVIRONMENT_HEADER

#include <rt/solids/solid.h>

namespace rt {

class Environment : public Solid {
public:

    Environment() {}
    Environment(CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float tmin = 0, float tmax = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
};

}


#endif