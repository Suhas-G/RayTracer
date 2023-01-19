#include <rt/coordmappers/tmapper.h>
#include <core/interpolate.h>
#include <rt/intersection.h>
namespace rt {

TriangleMapper::TriangleMapper(Point ntv[3])
{
    v0 = ntv[0];
    v1 = ntv[1];
    v2 = ntv[2];
}

TriangleMapper::TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2)
: v0(tv0), v1(tv1), v2(tv2)
{

}

Point TriangleMapper::getCoords(const Intersection& hit) const {
    Point local = hit.local();
    return Point(local.x * v0.x + local.y * v1.x + local.z * v2.x, local.x * v0.y + local.y * v1.y + local.z * v2.y, 0.0f);
}

}