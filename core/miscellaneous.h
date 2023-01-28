#ifndef CG1RAYTRACER_MISCELLANEOUS_HEADER
#define CG1RAYTRACER_MISCELLANEOUS_HEADER

#include <tuple>
#include <core/color.h>
#include <core/point.h>

namespace rt
{
    template<typename T>
    T sign(T a);

    std::tuple<bool, float, float> solveQuadratic(const float a, const float b, const float c);

    inline RGBColor debugColor() {return RGBColor(1.0f, 0.0f, 1.0f);}

    Point cartesianToSpherical(const Point &p);

    template<typename T>
    T clamp(T val, T min, T max);

    std::tuple<Vector, Vector> cameraForwardAndUp(const Point& from, const Point& to);
} 


#endif