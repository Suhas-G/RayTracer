#include <rt/solids/quad.h>
#include <rt/solids/triangle.h>
#include <rt/solids/infiniteplane.h>
#include <core/miscellaneous.h>
#include <core/random.h>
namespace rt {

Quad::Quad(const Point& origin, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material)
: Solid(texMapper, material), origin(origin), span1(span1), span2(span2) {
    normal = rt::cross(span1, span2);
    area = normal.length();
    normal = normal.normalize();
    invSpan1LengthSqr = 1.0f / span1.lensqr();
    invSpan2LengthSqr = 1.0f / span2.lensqr();
    b = origin + span1;
    c = origin + span2;
    d = origin + (span1 + span2);
    bounds = BBox(rt::min(rt::min(origin, b), rt::min(c, d)), rt::max(rt::max(origin, b), rt::max(c, d)));

    this->t1 = new Triangle(origin, c, d, texMapper, material);
    this->t2 = new Triangle(d, b, origin, texMapper, material);
}

BBox Quad::getBounds() const {
    /* TODO */
    return bounds;
}



Intersection Quad::intersect(const Ray& ray, float tmin, float tmax) const {
    Intersection intersection = InfinitePlane::intersectWithPlane(ray, origin, normal, this, tmin, tmax);
    if (intersection) {
        Point hp = intersection.hitPoint();
        Vector localHp = hp - origin;
        Point bc1 = this->t1->getBarycentricCoordinates(hp);
        // If not intersecting, barycentric coordinates will be negative
        if (bc1.x >= 0) {
            Point local = Point(rt::dot(localHp, span1) * invSpan1LengthSqr, rt::dot(localHp, span2) * invSpan2LengthSqr, 0);
            intersection.setLocal(local);
            return intersection;
        }

        Point bc2 = this->t2->getBarycentricCoordinates(hp);
        // If not intersecting, barycentric coordinates will be negative
        if (bc2.x >= 0) {
            Point local = Point(rt::dot(localHp, span1) * invSpan1LengthSqr, rt::dot(localHp, span2) * invSpan2LengthSqr, 0);
            intersection.setLocal(local);
            return intersection;
        }
    }

    return Intersection::failure();
}

Solid::Sample Quad::sample() const {
    float a = rt::random();
    float b = rt::random();

    Point p = origin + (a * span1) + (b * span2);
    return Sample{p, normal};
}

float Quad::getArea() const {
    return area;
}

}
