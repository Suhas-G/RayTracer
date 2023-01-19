#include <rt/intersection.h>

namespace rt {

Intersection::Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& local)
: ray(ray), solid(solid), distance(distance) , _normal(normal), _local(local){
    /* TODO */
}

Intersection::operator bool() const {
    return solid != nullptr;
}

Intersection Intersection::failure() {
    return Intersection(std::numeric_limits<float>::infinity(), nullptr);
}

Point Intersection::hitPoint() const {
    return ray.getPoint(distance);
}

Vector Intersection::normal() const {
    return _normal;
}

Point Intersection::local() const {
    return _local;
}

void Intersection::setLocal(Point uv) {
    this->_local = uv;
}

}
