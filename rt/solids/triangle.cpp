#include <rt/solids/triangle.h>
#include <rt/solids/infiniteplane.h>
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
            intersection.setLocal(Point(a, b, c));
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
        return Point(a, b, c);
    }
    return Point(-1, -1, -1);
}

Solid::Sample Triangle::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float Triangle::getArea() const {
    return area;
}

}