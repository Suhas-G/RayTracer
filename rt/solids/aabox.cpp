#include <rt/solids/aabox.h>
#include <rt/solids/infiniteplane.h>

namespace rt {

AABox::AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material)
{
    min = rt::min(corner1, corner2);
    max = rt::max(corner1, corner2);
    center = static_cast<Vector>(min + (max - min) / 2);
}

BBox AABox::getBounds() const {
    /* TODO */ NOT_IMPLEMENTED;
}

Solid::Sample AABox::sample() const {
    NOT_IMPLEMENTED;
}

float AABox::getArea() const {
    Vector diagonal = max - min;
    return 2 * (diagonal.x * diagonal.y + diagonal.y * diagonal.z + diagonal.x * diagonal.z);
}

Intersection AABox::intersect(const Ray& ray, float tmin, float tmax) const {
    // TODO: Extermely bad code and inefficient
    // X planes
    Intersection intersection = Intersection::failure();
    Intersection temp = Intersection::failure();
    int count = 0;
    Point hitpoint;
    Point x1 = Point(min.x, center.y, center.z);
    Point x2 = Point(max.x, center.y, center.z);
    temp = InfinitePlane::intersectWithPlane(ray, x1, static_cast<Vector>(x1 - center).normalize(), this, tmin, tmax);
    if (temp) {
        hitpoint = temp.hitPoint();
        if (hitpoint.y >= min.y && hitpoint.y <= max.y && hitpoint.z >= min.z && hitpoint.z <= max.z) {
            count++;
            intersection = temp;
            temp.setLocal(hitpoint - center);
            tmax = intersection.distance;
        }
    }

    temp = InfinitePlane::intersectWithPlane(ray, x2, static_cast<Vector>(x2 - center).normalize(), this, tmin, tmax);
    if (temp) {
        hitpoint = temp.hitPoint();
        if (hitpoint.y >= min.y && hitpoint.y <= max.y && hitpoint.z >= min.z && hitpoint.z <= max.z) {
            count++;
            intersection = temp;
            temp.setLocal(hitpoint - center);
            tmax = intersection.distance;
        }
    }

    if (count == 2) {
        return intersection;
    }

    // Y Plane
    Point y1 = Point(center.x, min.y, center.z);
    Point y2 = Point(center.x, max.y, center.z);

    temp = InfinitePlane::intersectWithPlane(ray, y1, static_cast<Vector>(y1 - center).normalize(), this, tmin, tmax);
    if (temp) {
        hitpoint = temp.hitPoint();
        if (hitpoint.x >= min.x && hitpoint.x <= max.x && hitpoint.z >= min.z && hitpoint.z <= max.z) {
            count++;
            intersection = temp;
            temp.setLocal(hitpoint - center);
            tmax = intersection.distance;
        }
    }

    if (count == 2) {
        return intersection;
    }


    temp = InfinitePlane::intersectWithPlane(ray, y2, static_cast<Vector>(y2 - center).normalize(), this, tmin, tmax);
    if (temp) {
        hitpoint = temp.hitPoint();
        if (hitpoint.x >= min.x && hitpoint.x <= max.x && hitpoint.z >= min.z && hitpoint.z <= max.z) {
            count++;
            intersection = temp;
            temp.setLocal(hitpoint - center);
            tmax = intersection.distance;
        }
    }

    if (count == 2) {
        return intersection;
    }


    // Z plane

    Point z1 = Point(center.x, center.y, min.z);
    Point z2 = Point(center.x, center.y, max.z);

    temp = InfinitePlane::intersectWithPlane(ray, z1, static_cast<Vector>(z1 - center).normalize(), this, tmin, tmax);
    if (temp) {
        hitpoint = temp.hitPoint();
        if (hitpoint.x >= min.x && hitpoint.x <= max.x && hitpoint.y >= min.y && hitpoint.y <= max.y) {
            count++;
            intersection = temp;
            temp.setLocal(hitpoint - center);
            tmax = intersection.distance;
        }
    }

    if (count == 2) {
        return intersection;
    }

    temp = InfinitePlane::intersectWithPlane(ray, z2, static_cast<Vector>(z2 - center).normalize(), this, tmin, tmax);
    if (temp) {
        hitpoint = temp.hitPoint();
        if (hitpoint.x >= min.x && hitpoint.x <= max.x && hitpoint.y >= min.y && hitpoint.y <= max.y) {
            count++;
            intersection = temp;
            temp.setLocal(hitpoint - center);
            tmax = intersection.distance;
        }
    }

    if (count == 2) {
        return intersection;
    }

    // This is necessary because I'm modifying tmax every intersection. So for all points that are found first 
    // are also closest, there will be only one intersection
    return intersection;
}

}
