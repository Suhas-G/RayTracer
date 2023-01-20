#include <rt/coordmappers/cylindrical.h>
#include <core/scalar.h>
#include <rt/intersection.h>
namespace rt {

CylindricalCoordMapper::CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis)
: origin(origin) {
    yscale = 1.0f / longitudinalAxis.length();
    xscale = 1.0f / (2 * rt::pi * polarAxis.length());
    this->yAxis = longitudinalAxis.normalize();
    this->zAxis = rt::cross(polarAxis.normalize(), yAxis);
    this->xAxis = rt::cross(yAxis, zAxis).normalize();
}

Point CylindricalCoordMapper::getCoords(const Intersection& hit) const {
    Vector local = hit.hitPoint() - origin;
    
    float v = rt::dot(local, this->yAxis) * yscale;
    float xProj = rt::dot(local, this->xAxis);
    float zProj = rt::dot(local, this->zAxis);
    float theta = rt::pi + std::atan2(zProj, xProj);
    float u = theta * xscale;

    return Point(u, v, 0);
}

}