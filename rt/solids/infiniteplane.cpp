#include <rt/solids/infiniteplane.h>

namespace rt {

InfinitePlane::InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* texMapper, Material* material)
: Solid(texMapper, material), origin(origin), normal(normal.normalize()) {
    
}

BBox InfinitePlane::getBounds() const {
    /* TODO */ NOT_IMPLEMENTED;
}

Intersection InfinitePlane::intersect(const Ray& ray, float tmin, float tmax) const {
    return InfinitePlane::intersectWithPlane(ray, origin, normal, this, tmin, tmax);
}

Solid::Sample InfinitePlane::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float InfinitePlane::getArea() const {
    return std::numeric_limits<float>::infinity();
}

Intersection InfinitePlane::intersectWithPlane(const Ray& ray, const Point& origin, const Vector& normal, 
    const Solid* solid, float tmin, float tmax) {
    float denom = rt::dot(ray.d, normal);
    if (std::abs(denom) < rt::epsilon) {
        return Intersection::failure();
    } 

    float t = rt::dot(origin - ray.o, normal) / denom;
    if (t >= tmin && t <= tmax) {
        const Point hitpoint = ray.getPoint(t);
        // W.r.t plane, local coordinate of hitpoint is hitpoint - origin
        // Assumed that ray.d is unit vector, otherwise t cannot be the distance
        return Intersection(t, ray, solid, normal, static_cast<Point>(hitpoint - origin));
    }

    return Intersection::failure();
}

}
