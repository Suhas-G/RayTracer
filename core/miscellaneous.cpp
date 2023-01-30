#include <cmath>
#include <core/miscellaneous.h>
#include <core/assert.h>
#include <core/scalar.h>
#include <core/vector.h>


namespace rt
{
    template<typename T>
    T sign(T a) {
        if (a >= 0) {
            return 1;
        } else {
            return -1;
        }
    }

    std::tuple<bool, float, float> solveQuadratic(const float a, const float b, const float c) {

        if (rt::approxEquals(a, 0.0f) && rt::approxEquals(b, 0.0f)) {
            return std::make_tuple(false, 0.0f, 0.0f);
        } else if (rt::approxEquals(a, 0.0f) && !rt::approxEquals(b, 0.0f)) {
            float res = c/b;
            return std::make_tuple(true, res, res);
        } else if (rt::approxEquals(b, 0.0f) && rt::approxEquals(c, 0.0f)) {
            return std::make_tuple(true, 0.0f, 0.0f);
        }

        if (rt::approxEquals(a, 0.0f)) {
            if (rt::approxEquals(b, 0.0f)) {
                return std::make_tuple(false, 0.0f, 0.0f);
            } else {
                float res = c/b;
                return std::make_tuple(true, res, res);
            }
        } else {
            if (rt::approxEquals(b, 0.0f) && rt::approxEquals(c, 0.0f)) {
                return std::make_tuple(true, 0.0f, 0.0f);
            } else if (rt::approxEquals(b, 0.0f)) {
                float res = std::sqrt(c/a);
                return std::make_tuple(true, res, -res);
            }
        }

        float det = b*b - 4*a*c;
        if (det < 0) {
            return std::make_tuple(false, 0.0f, 0.0f);
        }
        float temp = -0.5f * (b + rt::sign(b) * std::sqrt(det));
        rt_assert(temp != 0);
        return std::make_tuple(true, temp / a, c / temp);

    }

    template <typename T>
    T clamp(T val, T min, T max) {
        return std::max(std::min(val, max), min);
    }

    Point cartesianToSpherical(const Point &p) {
        float dist = p.distance();
        float theta = std::acos(rt::clamp(p.y, -1.0f, 1.0f) / dist);
        float phi = std::atan2(p.z, p.x);
        phi = phi < 0 ? rt::pi + phi : phi;
        return Point(phi / (2 * rt::pi), theta / rt::pi, dist);
    }


    std::tuple<Vector, Vector> cameraForwardAndUp(const Point& from, const Point& to) {
        Vector forward = (to - from).normalize();
        Vector up = Vector(0.0f, -1.0f, 0.0f);
        // if (forward.x <= forward.y && forward.x <= forward.z) {
        //     up.y = -forward.z;
        //     up.z = forward.y;
        // } else if (forward.y <= forward.x && forward.y <= forward.z) {
        //     up.x = -forward.z;
        //     up.z = forward.x;
        // } else {
        //     up.x = -forward.y;
        //     up.y = forward.x;
        // }
        up = up.normalize();
        Vector right = rt::cross(up, forward).normalize();
        up = rt::cross(right, forward);

        return std::make_tuple(forward, up);
    }
}
