#include <rt/coordmappers/environment.h>
#include <rt/intersection.h>
#include <core/miscellaneous.h>

namespace rt {

    EnvironmentMapper::EnvironmentMapper(float uscale, float vscale)
    : uscale(uscale), vscale(vscale) {

    }

    Point EnvironmentMapper::getCoords(const Intersection& hit) const {
        Vector dir = hit.ray.d.normalize();
        Point p = rt::cartesianToSpherical(static_cast<Point>(dir));
        return Point(p.x * uscale, p.y * vscale, p.z);
    }
}