#include <rt/solids/disc.h>
#include <rt/solids/infiniteplane.h>

namespace rt {

Disc::Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material)
: center(center), normal(normal.normalize()), radius(radius), Solid(texMapper, material) {
    radiusSqr = radius * radius;
}

BBox Disc::getBounds() const {
    /* TODO */ NOT_IMPLEMENTED;
}

Intersection Disc::intersect(const Ray& ray, float tmin, float tmax) const {
    const Intersection intersection = InfinitePlane::intersectWithPlane(ray, center, normal, this, tmin, tmax);
    if (intersection && intersection.local().distanceSqr() <= radiusSqr ) {
        return intersection;
    }
    return Intersection::failure();
}

Solid::Sample Disc::sample() const {
    NOT_IMPLEMENTED;
}

float Disc::getArea() const {
    return rt::pi * radiusSqr;
}

}
