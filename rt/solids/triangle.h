#ifndef CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER
#define CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER

#include <rt/solids/solid.h>

namespace rt {

class Triangle : public Solid {
public:
    Triangle() {}
    Triangle(Point vertices[3], CoordMapper* texMapper, Material* material);
    Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float tmin = 0, float tmax = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
    Point getBarycentricCoordinates(const Point& p);
protected:
    Point *vertices;
    Vector normal;
    float area;
    Vector edge1;
    Vector edge2;
    Vector edge3;
    BBox bounds;
};

}

#endif