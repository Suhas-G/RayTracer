#include <cmath>
#include <core/miscellaneous.h>
#include <core/assert.h>


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
        if (a == 0 && b == 0) {
            return std::make_tuple(false, 0.0f, 0.0f);
        } else if (a == 0 && b != 0) {
            float res = c/b;
            return std::make_tuple(true, res, res);
        } else if (b == 0 && c == 0) {
            return std::make_tuple(true, 0.0f, 0.0f);
        }

        if (a == 0) {
            if (b == 0) {
                return std::make_tuple(false, 0.0f, 0.0f);
            } else {
                float res = c/b;
                return std::make_tuple(true, res, res);
            }
        } else {
            if (b == 0 && c == 0) {
                return std::make_tuple(true, 0.0f, 0.0f);
            } else if (b == 0) {
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
}
