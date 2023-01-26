#include <rt/integrators/recraytrace.h>
#include <rt/world.h>
#include <rt/intersection.h>
#include <rt/solids/solid.h>
#include <rt/coordmappers/coordmapper.h>
#include <rt/materials/material.h>
#include <rt/lights/light.h>
namespace rt {

bool RecursiveRayTracingIntegrator::textureMaterialPresent(const Solid* solid) const{
    if (solid->material != nullptr && solid->texMapper != nullptr) {
        return true;
    }
    std::cerr << "Solid does not have texture or material" << std::endl;
    return false;
}

RGBColor computeFromLightSources(const World* world, const Ray& ray, const Intersection& intersection, 
                                 const Vector& normal, const Point& local) {
    RGBColor color = RGBColor::rep(0.0f);
    Point hitPoint = intersection.hitPoint();
    for (const auto& light : world->light) {
        LightHit lightHit = light->getLightHit(hitPoint);
        Vector invLightHitDirection = -lightHit.direction;

        if (!(lightHit.distance > rt::epsilon && (rt::dot(ray.d, normal) * rt::dot(invLightHitDirection, normal)) >= 0)) {
            continue;
        }

        Ray shadowRay = Ray(hitPoint + (2 * rt::epsilon * normal), lightHit.direction);
        // TODO: Confirm that the shadow ray and the primary ray leave the surface on the same side. HOW?
        Intersection shadowIntersection = world->scene->intersect(shadowRay, 0.0f, lightHit.distance);

        // Second condition is important for environment mapper and directional light
        if (!shadowIntersection || !(shadowIntersection.distance < lightHit.distance)) {
            RGBColor intensity = light->getIntensity(lightHit);
            RGBColor reflectance = intersection.solid->material->getReflectance(local, normal, -ray.d, lightHit.direction);
            color = color + (intensity * reflectance);
        }
    }
    return color;
}

RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray& ray) const {
    return this->getRadianceRecursive(ray, 0);
}

RGBColor RecursiveRayTracingIntegrator::getRadianceRecursive(const Ray& ray, int depth) const {
    RGBColor color = RGBColor::rep(0.0f);

    if (depth > this->recursionDepth) {
        return color;
    }

    Intersection intersection = world->scene->intersect(ray);

    if (!intersection || !textureMaterialPresent(intersection.solid)) return color; // No intersection with world, or something wrong with the solid

    Vector normal = intersection.normal();
    Point local = intersection.solid->texMapper->getCoords(intersection);

    color = color + intersection.solid->material->getEmission(local, normal, -ray.d);

    if (intersection.distance == FLT_MAX) return color; // Environment object, which has only emission

    switch (intersection.solid->material->useSampling())
    {
    case rt::Material::SAMPLING_NOT_NEEDED:
        color = color + computeFromLightSources(world, ray, intersection, normal, local);
        break;
    case rt::Material::SAMPLING_ALL: 
    {
        rt::Material::SampleReflectance sampleReflectance = intersection.solid->material->getSampleReflectance(local, normal, -ray.d);
        Point hitPoint = intersection.hitPoint();
        Ray secondaryRay = Ray(hitPoint + (2 * rt::epsilon * sampleReflectance.direction), sampleReflectance.direction);
        color = color + (sampleReflectance.reflectance * getRadianceRecursive(secondaryRay, depth + 1));
    }
        break;
    case rt::Material::SAMPLING_SECONDARY:
        break;
    default:
        break;
    }

    return color;
}

void RecursiveRayTracingIntegrator::setRecursionDepth(int depth) {
    this->recursionDepth = depth;
}

}