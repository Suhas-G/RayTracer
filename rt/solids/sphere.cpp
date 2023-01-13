#include <rt/solids/sphere.h>
#include <core/miscellaneous.h>

namespace rt {

Sphere::Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material)
: Solid(texMapper, material), center(center), radius(radius), radiusSqr(radius * radius)
{
    area = 4 * pi * radiusSqr;
    bounds = BBox(center - radius, center + radius);
}

BBox Sphere::getBounds() const {
    return bounds;
}

Intersection Sphere::intersect(const Ray& ray, float tmin, float tmax) const {
    Vector vec = ray.o - center;
    bool exists;
    float x1, x2;
    std::tie (exists, x1, x2) = rt::solveQuadratic(ray.d.lensqr(), 2 * rt::dot(ray.d, vec), vec.lensqr() - radiusSqr);
    if (!exists) {
        return Intersection::failure();
    }
    float dist;
    bool x1InsideRange = (x1 >= tmin && x1 < tmax);
    bool x2InsideRange = (x2 >= tmin && x2 < tmax);
    if (!x1InsideRange && !x2InsideRange) {
        return Intersection::failure();
    } else if (x1InsideRange && !x2InsideRange) {
        dist = x1;
    } else if (!x1InsideRange && x2InsideRange) {
        dist = x2;
    } else {
        dist = std::min(x1, x2);
    }

    // In sphere normal at every point is vector connecting center to that point
    // Its also the local coordinate of the point
    Vector dir = ray.getPoint(dist) - center;
    return Intersection(dist, ray, this, dir.normalize(), ray.getPoint(dist));
}

Solid::Sample Sphere::sample() const {
	NOT_IMPLEMENTED;
}

float Sphere::getArea() const {
    return area;
}

}
