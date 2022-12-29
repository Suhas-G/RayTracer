#include <rt/integrators/raytrace.h>
#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>

namespace rt {

RGBColor RayTracingIntegrator::getRadiance(const Ray& ray) const {
    /* TODO */
    RGBColor color = RGBColor::rep(0.0f);
    Intersection intersection = world->scene->intersect(ray);
    if (intersection) {
        Vector normal = intersection.normal();
        Point local = intersection.local();
        RGBColor emittance = intersection.solid->material->getEmission(local, normal, -ray.d);
        Point hitPoint = intersection.hitPoint();
        for (const auto& light : world->light) {
            LightHit lightHit = light->getLightHit(hitPoint);
            if (lightHit.distance > rt::epsilon && rt::dot(ray.d, normal) * rt::dot(lightHit.direction, normal) >= 0) {
                Vector invLightHitDir = -lightHit.direction;
                Ray shadowRay = Ray(hitPoint + (2 * rt::epsilon * normal), invLightHitDir);
                // TODO: Confirm that the shadow ray and the primary ray leave the surface on the same side.
                Intersection shadowIntersection = world->scene->intersect(shadowRay, 0.0f, lightHit.distance);

                if (!shadowIntersection) {
                    RGBColor intensity = light->getIntensity(lightHit);
                    RGBColor reflectance = intersection.solid->material->getReflectance(local, normal, -ray.d, invLightHitDir);
                    color = color + (intensity * reflectance);
                }
            }
        }
        color = color + emittance;
    }
    return color;
}

}