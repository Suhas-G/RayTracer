#include <rt/lights/spotlight.h>

namespace rt {

SpotLight::SpotLight(const Point& position, const Vector& direction, float angle, float power, const RGBColor& intensity)
: PointLight(position, intensity), direction(direction.normalize()), cosAngle(std::cos(angle)), exp(power) {
    /* TODO */
}

RGBColor SpotLight::getIntensity(const LightHit& irr) const {
    /* TODO */
    float cosIncomingAngle = rt::dot(-irr.direction, this->direction);
    if (cosIncomingAngle >= this->cosAngle) {
        return ((intensity / (irr.distance * irr.distance)) * std::pow(cosIncomingAngle, exp));
    }
    return RGBColor::rep(0.0f);
}

}
