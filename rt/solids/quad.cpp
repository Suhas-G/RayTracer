#include <rt/solids/quad.h>
#include <rt/solids/infiniteplane.h>

namespace rt {

Quad::Quad(const Point& origin, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material)
: Solid(texMapper, material), origin(origin), span1(span1), span2(span2) {
    normal = rt::cross(span1, span2);
    area = normal.length();
    normal = normal.normalize();
    invSpan1LengthSqr = 1.0f / span1.lensqr();
    invSpan2LengthSqr = 1.0f / span2.lensqr();
}

BBox Quad::getBounds() const {
    /* TODO */ NOT_IMPLEMENTED;
}

Intersection Quad::intersect(const Ray& ray, float tmin, float tmax) const {
    Intersection intersection = InfinitePlane::intersectWithPlane(ray, origin, normal, this, tmin, tmax);
    if (intersection) {
        Vector p = static_cast<Vector>(intersection.local());
        float alpha = rt::dot(span1, p);
        float beta = rt::dot(span2, p);

        if (alpha >= -rt::epsilon && alpha <= span1.lensqr() && beta >= -rt::epsilon && beta <= span2.lensqr()) {
            intersection.setLocal(Point(alpha * invSpan1LengthSqr, beta * invSpan2LengthSqr, 1.0f));
            return intersection;
        }
    }

    return Intersection::failure();
}

Solid::Sample Quad::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float Quad::getArea() const {
    return area;
}

}
