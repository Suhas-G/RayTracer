#include <rt/lights/pointlight.h>
#include <core/scalar.h>
#include <core/vector.h>

namespace rt {

PointLight::PointLight(const Point& position, const RGBColor& intensity)
: position(position), intensity(intensity) {
    /* TODO */
}

LightHit PointLight::getLightHit(const Point& p) const {
    /* TODO */
    Vector dir = p - this->position;
    float len = dir.length();
    dir = dir.normalize();
    return LightHit({dir, len, dir});
}

RGBColor PointLight::getIntensity(const LightHit& irr) const {
    /* TODO */
    return this->intensity / (irr.distance * irr.distance);
}

}
