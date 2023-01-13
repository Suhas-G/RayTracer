#ifndef CG1RAYTRACER_MISCELLANEOUS_HEADER
#define CG1RAYTRACER_MISCELLANEOUS_HEADER

#include <tuple>
#include <core/color.h>

namespace rt
{
    template<typename T>
    T sign(T a);

    std::tuple<bool, float, float> solveQuadratic(const float a, const float b, const float c);

    inline RGBColor debugColor() {return RGBColor(1.0f, 0.0f, 1.0f);}
} 


#endif