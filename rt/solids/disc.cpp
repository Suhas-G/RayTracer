#include <rt/solids/disc.h>
#include <rt/solids/infiniteplane.h>

namespace rt {

Disc::Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material)
: Solid(texMapper, material), center(center), radius(radius), normal(normal.normalize()) {
    radiusSqr = radius * radius;
}

BBox Disc::getBounds() const {
    /* TODO */ NOT_IMPLEMENTED;
    auto uppoint = center + (normal * rt::epsilon * rt::OFFSET_MULTIPLIER);
    auto downpoint = center - (normal * rt::epsilon * rt::OFFSET_MULTIPLIER);
    return BBox(uppoint, downpoint);
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
