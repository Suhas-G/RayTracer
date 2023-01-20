#include <rt/solids/environment.h>
#include <rt/bbox.h>
#include <core/miscellaneous.h>

namespace rt {

    Environment::Environment(CoordMapper* texMapper, Material* material)
    : Solid(texMapper, material){

    }

    BBox Environment::getBounds() const {
        return BBox::full();
    }

    Intersection Environment::intersect(const Ray& ray, float tmin, float tmax) const {
        if (tmax == FLT_MAX) {
            Point local = rt::cartesianToSpherical(static_cast<Point>(ray.d));
            return Intersection(FLT_MAX, ray, this, (-ray.d).normalize(), local);
        }
        return Intersection::failure();
    }

    float Environment::getArea() const {
        return std::numeric_limits<float>::infinity();
    }

    Solid::Sample Environment::sample() const {
        /* TODO */ NOT_IMPLEMENTED;
    }
}