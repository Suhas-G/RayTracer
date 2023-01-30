#ifndef CG1RAYTRACER_LIGHTS_AREALIGHT_HEADER
#define CG1RAYTRACER_LIGHTS_AREALIGHT_HEADER

#include <rt/solids/solid.h>
#include <rt/lights/light.h>

namespace rt {

class AreaLight : public Light {
public:
    AreaLight() {}
    AreaLight(Solid* source, float scale = 1.0f);
    virtual LightHit getLightHit(const Point& p) const;
    virtual RGBColor getIntensity(const LightHit& irr) const;
private:
    Solid* source;
    float scale = 1.0f;
};

}

#endif
