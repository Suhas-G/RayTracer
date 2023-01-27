#include <rt/solids/triangle.h>
#include <rt/solids/infiniteplane.h>
#include <core/random.h>
namespace rt {

Triangle::Triangle(Point vertices[3], CoordMapper* texMapper, Material* material)
: Solid(texMapper, material), vertices(vertices)
{
    edge1 = vertices[1] - vertices[0];
    edge2 = vertices[2] - vertices[1];
    edge3 = vertices[0] - vertices[2];
    normal = rt::cross(edge1, -edge3);
    area = normal.length() / 2.0f;
    normal = normal.normalize();

    bounds = BBox(rt::min(vertices[0], vertices[1], vertices[2]), rt::max(vertices[0], vertices[1], vertices[2]));
}

Triangle::Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material)
: Solid(texMapper, material)
{
    /* TODO */
    vertices = new Point[3]{v1, v2, v3};
    edge1 = vertices[1] - vertices[0];
    edge2 = vertices[2] - vertices[1];
    edge3 = vertices[0] - vertices[2];
    normal = rt::cross(edge1, -edge3);
    area = normal.length() / 2.0f;
    normal = normal.normalize();

    bounds = BBox(rt::min(vertices[0], vertices[1], vertices[2]), rt::max(vertices[0], vertices[1], vertices[2]));

}

BBox Triangle::getBounds() const {
    /* TODO */
    return bounds;
}

Intersection Triangle::intersect(const Ray& ray, float tmin, float tmax) const {
    /* TODO */
    Intersection intersection = InfinitePlane::intersectWithPlane(ray, vertices[0], normal, this, tmin, tmax);
    if (intersection) {
        Point p = ray.getPoint(intersection.distance);
        Vector C1 = p - vertices[0]; 
        Vector C2 = p - vertices[1]; 
        Vector C3 = p - vertices[2];
        float a = rt::dot(normal, rt::cross(edge1, C1));
        float b = rt::dot(normal, rt::cross(edge2, C2));
        float c = rt::dot(normal, rt::cross(edge3, C3));
        if (a > 0 && b > 0 && c > 0) {
            // FIXME: WHY??? If used (a, b, c) instead of (b, c, a) the local image output is wrong
            Point local = Point(a, b, c);
            // Normalise by the length of the normal. https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates.html
            intersection.setLocal(local * (1.0f / (area * 2.0f)));
            return intersection;
        }
    }
    return Intersection::failure();
}

Point Triangle::getBarycentricCoordinates(const Point& p) {
    /* TODO */
    Vector C1 = p - vertices[0]; 
    Vector C2 = p - vertices[1]; 
    Vector C3 = p - vertices[2];
    float a = rt::dot(normal, rt::cross(edge1, C1));
    float b = rt::dot(normal, rt::cross(edge2, C2));
    float c = rt::dot(normal, rt::cross(edge3, C3));
    if (a > 0 && b > 0 && c > 0) {
        Point local = Point(b, c, a)* (1.0f / (area * 2.0f));
        // return Point(a, b, c);
        return local;
    }
    return Point(-1, -1, -1);
}

Solid::Sample Triangle::sample() const {
    float a = rt::random();
    float b = rt::random(1 - a);
    float c = 1 - a - b;

    Point p = a * vertices[0] + b * vertices[1] + c * vertices[2];

    return Sample{p, normal};
}

float Triangle::getArea() const {
    return area;
}

}