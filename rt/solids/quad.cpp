#include <rt/solids/quad.h>
#include <rt/solids/triangle.h>
#include <rt/solids/infiniteplane.h>
#include <core/miscellaneous.h>

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
    // Intersection t1_intersect = t1->intersect(ray, tmin, tmax);
    // if (t1_intersect) {
    //     Vector hp = t1_intersect.hitPoint() - origin;
    //     Point local = Point(dot(hp, span1) / span1.lensqr(), dot(hp, span2) / span2.lensqr(), 0);
    //     return Intersection(t1_intersect.distance, ray, this, t1_intersect.normal(), local);
    // }
    
    // Intersection t2_intersect = t2->intersect(ray, tmin, tmax);
    // if (t2_intersect) {
    //     Vector hp = t2_intersect.hitPoint() - origin;
    //     Point local = Point(dot(hp, span1) / span1.lensqr(), dot(hp, span2) / span2.lensqr(), 0);
    //     return Intersection(t2_intersect.distance, ray, this, t2_intersect.normal(), local);
    // }

    return Intersection::failure();
}

Solid::Sample Quad::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float Quad::getArea() const {
    return area;
}

}
