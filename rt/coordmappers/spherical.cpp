#include <core/scalar.h>
#include <rt/intersection.h>
#include <rt/coordmappers/spherical.h>

namespace rt {

SphericalCoordMapper::SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef)
: origin(origin) {
    vscale = 1.0f / (rt::pi * zenith.length());
    uscale = 1.0f / (2 * rt::pi * azimuthRef.length());
    transformation = Matrix::system(azimuthRef.normalize(), zenith.normalize(), rt::cross(azimuthRef, zenith).normalize()).invert();
}

Point SphericalCoordMapper::getCoords(const Intersection& hit) const {
    Vector local = transformation * (hit.hitPoint() - origin);


    float dist = local.length();
    float phi = std::acos(local.y / dist);
    float v = phi * vscale;
    float theta = rt::pi + std::atan2(local.z, local.x);
    float u = theta * uscale;

    return Point(u, v, dist);
}

}