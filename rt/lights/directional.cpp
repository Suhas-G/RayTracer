#include <rt/lights/directional.h>
#include <core/scalar.h>
#include <core/vector.h>

namespace rt {

DirectionalLight::DirectionalLight(const Vector& direction, const RGBColor& color)
: direction(direction.normalize()), color(color) {
    /* TODO */
}

LightHit DirectionalLight::getLightHit(const Point& p) const {
    CG_UNUSED(p);
    /* TODO */
    return LightHit{direction, FLT_MAX, direction};
}

RGBColor DirectionalLight::getIntensity(const LightHit& irr) const {
    CG_UNUSED(irr);
    /* TODO */
    return this->color;
}

}
