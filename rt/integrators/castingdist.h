#ifndef CG1RAYTRACER_INTEGRATORS_CASTINGDIST_HEADER
#define CG1RAYTRACER_INTEGRATORS_CASTINGDIST_HEADER

#include <rt/integrators/integrator.h>

namespace rt {

class RayCastingDistIntegrator : public Integrator {
public:
    RayCastingDistIntegrator(World* world, const RGBColor& nearColor, float nearDist, const RGBColor& farColor, float farDist);
    virtual RGBColor getRadiance(const Ray& ray) const;
private:
    const RGBColor nearColor;
    const RGBColor farColor;
    const float nearDist;
    const float farDist;
};

}

#endif