#include <rt/integrators/castingdist.h>
#include <rt/world.h>

namespace rt {

RayCastingDistIntegrator::RayCastingDistIntegrator(World* world, const RGBColor& nearColor, float nearDist, const RGBColor& farColor, float farDist)
    : Integrator(world), nearColor(nearColor), farColor(farColor), nearDist(nearDist), farDist(farDist)
{
    /* TODO */
}

RGBColor RayCastingDistIntegrator::getRadiance(const Ray& ray) const {
    /* TODO */
    Intersection intersection = world->scene->intersect(ray);
    if (intersection) {
        float alpha = rt::clamp01((intersection.distance - nearDist) / (farDist - nearDist));
        RGBColor color = RGBColor::lerp(nearColor, farColor, alpha);
        // Inverting normal direction, because the normal at the hitpoint should be opposite to ray direction, then the
        // light reflected is highest
        return color * rt::dot(ray.d, -intersection.normal());
    }

    return RGBColor::rep(0.0f);
}

}
