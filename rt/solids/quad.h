#ifndef CG1RAYTRACER_SOLIDS_QUAD_HEADER
#define CG1RAYTRACER_SOLIDS_QUAD_HEADER

#include <rt/solids/triangle.h>
#include <rt/solids/solid.h>

namespace rt {

class Quad : public Solid {
public:
    Quad() {}
    Quad(const Point& v1, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float tmin = 0, float tmax = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
private:
    Point origin;
    Vector span1;
    Vector span2;
    Vector normal;
    float area;
    float invSpan1LengthSqr;
    float invSpan2LengthSqr;
    BBox bounds;
    Point b;
    Point c;
    Point d;
    Triangle *t1;
    Triangle *t2;
};

}

#endif