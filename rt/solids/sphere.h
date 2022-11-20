#ifndef CG1RAYTRACER_SOLIDS_SPHERE_HEADER
#define CG1RAYTRACER_SOLIDS_SPHERE_HEADER

#include <rt/solids/solid.h>

namespace rt {

class Sphere : public Solid {
public:
    Sphere(): center(Point()), radius(0), radiusSqr(0) {}
    Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float tmin = 0, float tmax = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
private:
    const Point center;
    const float radius;
    const float radiusSqr;
    float area = 0.0f;
};

}


#endif