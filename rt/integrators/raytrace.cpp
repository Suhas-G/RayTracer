#include <rt/integrators/raytrace.h>
#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {

RGBColor RayTracingIntegrator::getRadiance(const Ray& ray) const {
    /* TODO */
    RGBColor color = RGBColor::rep(0.0f);
    Intersection intersection = world->scene->intersect(ray);
    if (intersection) {
        Vector normal = intersection.normal();
        Point local;
        rt_assert(intersection.solid->texMapper != nullptr);
        local = intersection.solid->texMapper->getCoords(intersection);
        RGBColor emittance = RGBColor::rep(0.0f);
        if (intersection.solid->material != nullptr) {
            emittance = intersection.solid->material->getEmission(local, normal, -ray.d);
            Point hitPoint = intersection.hitPoint();
            if (intersection.distance < FLT_MAX) {
                for (const auto& light : world->light) {
                    LightHit lightHit = light->getLightHit(hitPoint);
                    Vector invLightHitDirection = -lightHit.direction;
                    if (dot(normal, ray.d.normalize()) > 0) {
                        normal = -normal;
                        std::cout << "Inverting normal" << std::endl;
                    }
                    if (lightHit.distance > rt::epsilon && rt::dot(ray.d, normal) * rt::dot(invLightHitDirection, normal) >= 0) {
                        Ray shadowRay = Ray(hitPoint + (2 * rt::epsilon * normal), lightHit.direction);
                        // TODO: Confirm that the shadow ray and the primary ray leave the surface on the same side. HOW?
                        Intersection shadowIntersection = world->scene->intersect(shadowRay, 0.0f, lightHit.distance);

                        // Second condition is important for environment mapper and directional light
                        if (!shadowIntersection || !(shadowIntersection.distance < lightHit.distance)) {
                            RGBColor intensity = light->getIntensity(lightHit);
                            RGBColor reflectance = intersection.solid->material->getReflectance(local, normal, -ray.d, lightHit.direction);
                            // FIXME: The last cosine term has to be checked later
                            color = color + (intensity * reflectance * rt::dot(invLightHitDirection, lightHit.normal));
                        }
                    }
                }
            }
            color = color + emittance;
        } else {
            std::cout << "Here" << std::endl;
        }
    }
    return color;
}

}