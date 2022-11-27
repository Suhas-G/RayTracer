#include <core/assert.h>
#include <core/macros.h>
#include <rt/ray.h>

namespace rt {

Ray::Ray(const Point& o, const Vector& d): o(o), d(d)
{
    rt_assert(d != Vector::rep(0.0f));

    invD = 1 / this->d.normalize();
    dirIsNeg[0] = d.x < 0;
    dirIsNeg[1] = d.y < 0;
    dirIsNeg[2] = d.z < 0;
}

Point Ray::getPoint(float distance) const {
    return o + distance * d;
}

}
