#include <rt/solids/striangle.h>
#include <rt/intersection.h>

namespace rt {

SmoothTriangle::SmoothTriangle(Point vertices[3], Vector normals[3], CoordMapper* texMapper, Material* material)
:Triangle(vertices, texMapper, material), normals(normals)
{
    /* TODO */
}

SmoothTriangle::SmoothTriangle(const Point& v1, const Point& v2, const Point& v3, const Vector& n1, const Vector& n2, const Vector& n3, CoordMapper* texMapper, Material* material)
:Triangle(v1, v2, v3, texMapper, material)
{
    this->normals = new Vector[3]{n1, n2, n3};
}

Intersection SmoothTriangle::intersect(const Ray& ray, float tmin, float tmax) const {
    Intersection intersection = Triangle::intersect(ray, tmin, tmax);
    if (intersection) {
        Point local = intersection.local();
        Vector fakeNormal = (local.x * this->normals[0]) + (local.y * this->normals[1]) + (local.z * this->normals[2]);
        intersection.setNormal(fakeNormal);
    }

    return intersection;
}

}