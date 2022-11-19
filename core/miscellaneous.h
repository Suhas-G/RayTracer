#ifndef CG1RAYTRACER_MISCELLANEOUS_HEADER
#define CG1RAYTRACER_MISCELLANEOUS_HEADER

#include <tuple>

namespace rt
{
    template<typename T>
    T sign(T a);

    std::tuple<bool, float, float> solveQuadratic(const float a, const float b, const float c);

} 


#endif