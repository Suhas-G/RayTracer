#include <core/assert.h>
#include <core/macros.h>
#include <rt/ray.h>
#include <cmath>

namespace rt {

Ray::Ray(const Point& o, const Vector& d): o(o), d(d)
{
    rt_assert(d != Vector::rep(0.0f));

    invD = 1.0f / this->d.normalize();
    dirIsNeg[0] = std::signbit(d.x) ? 1 : 0;
    dirIsNeg[1] = std::signbit(d.y) ? 1 : 0;
    dirIsNeg[2] = std::signbit(d.z) ? 1 : 0;
}

Point Ray::getPoint(float distance) const {
    return o + distance * d;
}

}
