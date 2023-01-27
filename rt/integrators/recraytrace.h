#ifndef CG1RAYTRACER_INTEGRATORS_RECURSIVERAYTRACING_HEADER
#define CG1RAYTRACER_INTEGRATORS_RECURSIVERAYTRACING_HEADER

#include <rt/integrators/integrator.h>

namespace rt {

class Solid;

class RecursiveRayTracingIntegrator : public Integrator {
public:
    RecursiveRayTracingIntegrator(World* world) : Integrator(world) {}
    virtual RGBColor getRadiance(const Ray& ray) const;

    void setRecursionDepth(int depth);

private:
    bool textureMaterialPresent(const Solid* solid) const;
    RGBColor getRadianceRecursive(const Ray& ray, int depth) const;
    int recursionDepth = 6;
};

}

#endif