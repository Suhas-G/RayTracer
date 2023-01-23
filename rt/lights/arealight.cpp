#include <rt/lights/arealight.h>
#include <rt/materials/material.h>
#include <core/color.h>

namespace rt {

LightHit AreaLight::getLightHit(const Point& p) const {
    rt::Solid::Sample sample = source->sample();
    Vector dir = sample.point - p;
    // A small offset from the surface in the direction of object to light, so that this solid
    // is not considered for shadow intersection
    float len = (dir - (10 * rt::epsilon * dir)).length();
    dir = dir.normalize();
    if (rt::dot(dir, sample.normal) < 0) {
        sample.normal = -sample.normal;
    }

    return LightHit{dir, len, sample.normal};
}

RGBColor AreaLight::getIntensity(const LightHit& irr) const {
    // FIXME: Dummy values used for now
    float val = std::abs(rt::dot(-irr.direction, irr.normal));
    RGBColor emission = source->material->getEmission(Point::rep(0.0f), Vector::rep(0.0f), Vector::rep(0.0f));
    float lightArea = std::abs(source->getArea());
    return  (emission * lightArea * val) / (irr.distance * irr.distance);
}

AreaLight::AreaLight(Solid* source)
:source(source)
{
    /* TODO */
}

}