#ifndef CG1RAYTRACER_NORMALMAPPERS_RANDOMROUGH_HEADER
#define CG1RAYTRACER_NORMALMAPPERS_RANDOMROUGH_HEADER

#include <rt/normalmappers/normalmap.h>

namespace rt {

class RandomRoughNormalMapper: public NormalMapper {
public:
    RandomRoughNormalMapper(float amplitude);
    Vector computePerturbation(const Intersection& hit) const override;
private:
    float amplitude = 1.0f;
};

}

#endif