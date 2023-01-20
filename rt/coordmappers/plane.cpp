#include <rt/intersection.h>
#include <rt/coordmappers/plane.h>

namespace rt {

PlaneCoordMapper::PlaneCoordMapper(const Vector& e1, const Vector& e2)
{

    transformation = Matrix::system(e1, e2, rt::cross(e1, e2));
    transformation = transformation.invert();

}

Point PlaneCoordMapper::getCoords(const Intersection& hit) const {
    Point p = transformation * hit.local();
    return p;
}

}