#include <core/scalar.h>
#include <rt/intersection.h>
#include <rt/coordmappers/spherical.h>

namespace rt {

SphericalCoordMapper::SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef)
: origin(origin) {
    vscale = 1.0f / (rt::pi * zenith.length());
    uscale = 1.0f / (2 * rt::pi * azimuthRef.length());
    Vector xAxis = azimuthRef.normalize();
    Vector yAxis = zenith.normalize();
    Vector zAxis = rt::cross(xAxis, yAxis);
    xAxis = rt::cross(yAxis, zAxis);
    transformation = Matrix::system(xAxis, yAxis, zAxis).invert();
}

Point SphericalCoordMapper::getCoords(const Intersection& hit) const {
    Vector local = transformation * (hit.hitPoint() - origin);


    float dist = local.length();
    float phi = rt::pi + std::atan2(std::sqrt((local.x * local.x) + (local.z * local.z)), local.y);
    float v = phi * vscale;
    float theta = rt::pi + std::atan2(local.z, local.x);
    float u = theta * uscale;

    return Point(u, v, dist);
}

}