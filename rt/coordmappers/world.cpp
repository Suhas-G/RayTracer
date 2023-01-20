#include <rt/coordmappers/world.h>
#include <rt/intersection.h>
namespace rt {

Point WorldMapper::getCoords(const Intersection& hit) const {
    Point p = hit.hitPoint();
    return Point(p.x * scale.x, p.y * scale.y, p.z * scale.z);
}

WorldMapper::WorldMapper()
: scale(Vector(1, 1, 1))
{
    /* TODO */
}

WorldMapper::WorldMapper(const Vector& scale)
: scale(scale)
{
    /* TODO */
}

}