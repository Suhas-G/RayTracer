#include <rt/normalmappers/randomrough.h>
#include <core/random.h>

namespace rt
{
    RandomRoughNormalMapper::RandomRoughNormalMapper(float amplitude)
    : NormalMapper(), amplitude(amplitude) {

    }

    Vector RandomRoughNormalMapper::computePerturbation(const Intersection& hit) const {
        return Vector(rt::random(-amplitude, amplitude), rt::random(-amplitude, amplitude), rt::random(-amplitude, amplitude));
    }
}
