#ifndef CG1RAYTRACER_INTERSECTION_HEADER
#define CG1RAYTRACER_INTERSECTION_HEADER

#include <core/scalar.h>
#include <core/vector.h>
#include <rt/ray.h>

namespace rt {

class Solid;

class Intersection {
public:
    Ray ray;
    const Solid* solid;
    float distance;

    Intersection() {}
    Intersection(float distance, Solid* solid): solid(solid), distance(distance) {}
    static Intersection failure();
    Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& uv);

    Point hitPoint() const;
    Vector normal() const;
    Point local() const;

    void setLocal(Point uv);
    void setNormal(Vector normal);

    operator bool() const; //this allows intersection object to be put directly in conditional statements. Becomes true if there is an intersection

private:
    Vector _normal;
    Point _local;

};

}

#endif