#include <core/scalar.h>
#include <rt/intersection.h>
#include <rt/coordmappers/spherical.h>

namespace rt {

SphericalCoordMapper::SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef)
: origin(origin) {
    vscale = 1.0f / (rt::pi * zenith.length());
    uscale = 1.0f / (2 * rt::pi * azimuthRef.length());
    this->yAxis = zenith.normalize();
    this->zAxis = rt::cross(azimuthRef.normalize(), yAxis);
    this->xAxis = rt::cross(yAxis, zAxis).normalize();
    // this->xAxis = azimuthRef.normalize();
}

Point SphericalCoordMapper::getCoords(const Intersection& hit) const {
    Vector local = hit.hitPoint() - origin;
    float dist = local.length();
    float phi = std::acos(rt::dot(local, -this->yAxis) / dist);
    float v = phi  * vscale;
    float xProj = rt::dot(local, this->xAxis);
    float zProj = rt::dot(local, this->zAxis);
    float theta = rt::pi + std::atan2(xProj, zProj);
    float u = theta * uscale;

    return Point(u, v, 0);
}

}