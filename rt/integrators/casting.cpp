#include <rt/integrators/casting.h>
#include <rt/world.h>

namespace rt {

RGBColor RayCastingIntegrator::getRadiance(const Ray& ray) const {
    Intersection intersection = world->scene->intersect(ray);
    if (intersection) {
        // Inverting normal direction, because the normal at the hitpoint should be opposite to ray direction, then the
        // light reflected is highest
        float d = -rt::dot(ray.d, intersection.normal());
        return RGBColor::rep(d);
    }

    return RGBColor::rep(0.0f);
}

}
