#include <cmath>
#include <rt/bbox.h>
#include <rt/ray.h>
#include <core/scalar.h>
namespace rt {

    BBox BBox::empty() {
        Point p1 = Point(0, 0, 0);
        Point p2 = Point(0, 0, 0);
        BBox box;
        box.min = p1;
        box.max = p2;
        return box;
    }

    BBox BBox::full() {
        Point p1 = Point::rep(-std::numeric_limits<float>::infinity());
        Point p2 = Point::rep(std::numeric_limits<float>::infinity());
        BBox box(p1, p2);
        return box;
    }


    void BBox::extend(const Point& point) {
        // No need to extend if its full
        // cant extend empty by a point
        if (isEmpty()) {
            this->min = point;
            this->max = point;
        } else if (!isFull()) {
            this->min = rt::min(this->min, point);
            this->max = rt::max(this->max, point);
            ensureThickness();
        }
    }

    void BBox::extend(const BBox& bbox) {
        if (isEmpty()) {
            this->min = bbox.min;
            this->max = bbox.max;
        } else if (!isFull()) {
            this->min = rt::min(this->min, bbox.min);
            this->max = rt::max(this->max, bbox.max);
            ensureThickness();
        }
    }

    std::pair<float, float> BBox::intersect(const Ray& ray) const {
        /* TODO */
        if (isEmpty()) {
            return std::make_pair(1.0f, -1.0f);
        }
        if (isFull()) {
            return std::make_pair(-std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
        }

        const Point* bounds[2] = { &min, &max };
        float tmin = ((*bounds)[ray.dirIsNeg[0]].x - ray.o.x) * ray.invD.x;
        float tmax = ((*bounds)[1 - ray.dirIsNeg[0]].x - ray.o.x) * ray.invD.x;
        float tymin = ((*bounds)[ray.dirIsNeg[1]].y - ray.o.y) * ray.invD.y;
        float tymax = ((*bounds)[1 - ray.dirIsNeg[1]].y - ray.o.y) * ray.invD.y;
        // // Do not intersect
        if ((tmin > tymax) || (tymin > tmax)) {
            // Maybe has to be changed to actual t values
            return std::make_pair(1.0f, -1.0f);
        }

        if (tymin > tmin) tmin = tymin;
        if (tymax < tmax) tmax = tymax;

        float tzmin = ((*bounds)[ray.dirIsNeg[2]].z - ray.o.z) * ray.invD.z;
        float tzmax = ((*bounds)[1 - ray.dirIsNeg[2]].z - ray.o.z) * ray.invD.z;

        if ((tmin > tzmax) || (tzmin > tmax)) {
            // Maybe has to be changed to actual t values
            return std::make_pair(1.0f, -1.0f);
        }

        if (tzmin > tmin) tmin = tzmin;
        if (tzmax < tmax) tmax = tzmax;

        // rt_assert(tmin >= tmax);

        return std::make_pair(tmin, tmax);
    }

    bool BBox::isUnbound() const {
        /* TODO */
        return (std::isinf(min.x) || std::isinf(min.y) || std::isinf(min.z) ||
            std::isinf(max.x) || std::isinf(max.y) || std::isinf(max.z));
    }

    Axis BBox::biggestDimensionAxis() const {

        float xDim, yDim, zDim;
        xDim = max.x - min.x;
        yDim = max.y - min.y;
        zDim = max.z - min.z;

        if ((xDim - yDim) > rt::epsilon && (xDim - zDim) > rt::epsilon) {
            return Axis::X;
        }
        if ((yDim - xDim) > rt::epsilon && (yDim - zDim) > rt::epsilon) {
            return Axis::Y;
        }
        
        // Rest of the cases even when equal
        return Axis::Z;
    }

    bool BBox::isEmpty() const {
        return (std::abs(max.x) < rt::epsilon && std::abs(max.y) < rt::epsilon && std::abs(max.z) < rt::epsilon &&
                std::abs(min.x) < rt::epsilon && std::abs(min.y) < rt::epsilon && std::abs(min.z) < rt::epsilon);
    }

    bool BBox::isFull() const {
        return (std::isinf(min.x) && std::isinf(min.y) && std::isinf(min.z) &&
            std::isinf(max.x) && std::isinf(max.y) && std::isinf(max.z));
    }

    void BBox::ensureThickness() {
        for (int i = 0; i < 3; i++) {
            if (this->max[i] - this->min[i] < rt::epsilon) {
                this->max[i] += (rt::epsilon);
                this->min[i] -= (rt::epsilon);
            }
        }
    }

    Point BBox::center() const {
        Point c;
        // TODO: If only one side is infinity, then how to take center?
        if (std::isinf(min.x) || std::isinf(max.x)) {
            c.x = 0;
        } else {
            c.x = 0.5f * (min.x + max.x);
        }

        if (std::isinf(min.y) || std::isinf(max.y)) {
            c.y = 0;
        } else {
            c.y = 0.5f * (min.y + max.y);
        }

        if (std::isinf(min.z) || std::isinf(max.z)) {
            c.z = 0;
        } else {
            c.z = 0.5f * (min.z + max.z);
        }

        return c;
    }

    std::vector<Point> BBox::getCorners() const {
        return std::vector<Point>({Point(min.x, min.y, min.z), Point(min.x, min.y, max.z), Point(min.x, max.y, min.z),
                                    Point(min.x, max.y, max.z), Point(max.x, min.y, min.z), Point(max.x, min.y, max.z),
                                    Point(max.x, max.y, min.z), Point(max.x, max.y, max.z)});
    }

    std::ostream& operator<<(std::ostream& os, const BBox& box) {
        return os << "BBox(min: " << box.min << ", max: " << box.max << ")";
    }

}