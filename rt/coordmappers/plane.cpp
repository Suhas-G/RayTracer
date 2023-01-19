#include <rt/intersection.h>
#include <rt/coordmappers/plane.h>

namespace rt {

PlaneCoordMapper::PlaneCoordMapper(const Vector& e1, const Vector& e2)
{

    transformation = Matrix::system(e1, e2, rt::cross(e1, e2));
    transformation = transformation.invert();

}

Point PlaneCoordMapper::getCoords(const Intersection& hit) const {
    // Vector localHp = static_cast<Vector>(hit.local());
    // Vector n = rt::cross(e1, e2).normalize();
    // float distance = rt::dot(localHp, n);
    // localHp = static_cast<Vector>(hit.hitPoint()) - (distance * n);
    // // FIXME: Its not correct. Something needs to be changed
    // return Point(std::abs(rt::dot(localHp.normalize(), e1.normalize())), std::abs(rt::dot(localHp.normalize(), e2.normalize())), 0.0f);
    
    Point p = transformation * hit.local();

    return p;
}

}