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
    Point b, c, d;
    b = origin + span1;
    c = origin + span2;
    d = origin + (span1 + span2);
    bounds = BBox(rt::min(rt::min(origin, b), rt::min(c, d)), rt::max(rt::max(origin, b), rt::max(c, d)));
}

BBox Quad::getBounds() const {
    /* TODO */
    return bounds;
}

Intersection Quad::intersect(const Ray& ray, float tmin, float tmax) const {
    Intersection intersection = InfinitePlane::intersectWithPlane(ray, origin, normal, this, tmin, tmax);
    if (intersection) {
        Vector p = static_cast<Vector>(intersection.local());
        float alpha, beta;
        float x1 = span1.x, y1 = span1.y, z1 = span1.z, x2 = span2.x, y2 = span2.y, z2 = span2.z, px = p.x, py = p.y, pz = p.z;
        if (std::abs(x2 * y1 - x1 * y2) > rt::epsilon) {
            beta = (px * y1 - py * x1) / (x2 * y1 - x1 * y2);
        } else if (std::abs(x2 * z1 - x1 * z2) > rt::epsilon) {
            beta = (px * z1 - pz * x1) / (x2 * z1 - x1 * z2);
        } else {
            // TODO: Is this correct??
            beta = 0.0f;
        }

        if (std::abs(x1) > rt::epsilon) {
            alpha = (px - beta * x2) / x1;
        } else if (std::abs(y1) > rt::epsilon) {
            alpha = (py - beta * y2) / y1;
        } else if (std::abs(z1) > rt::epsilon) {
            alpha = (pz - beta * z2) / z1;
        } else {
            alpha = 0.0f;
        }

        if (alpha >= 0.0f && alpha <= 1.0f && beta >= 0.0f && beta <= 1.0f) {
            intersection.setLocal(Point(alpha, beta, 1.0f));
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
