#ifndef CG1RAYTRACER_BBOX_HEADER
#define CG1RAYTRACER_BBOX_HEADER

#include <utility>
#include <core/point.h>
#include <core/vector.h>
#include <vector>

namespace rt {

enum class Axis {
    X = 0, Y = 1, Z = 2
};

class Ray;

class BBox {
public:
    Point min, max;

    BBox() {}
    BBox(const Point& min, const Point& max)
    {
        this->min = rt::min(min, max);
        this->max = rt::max(min, max);
        ensureThickness();
    }

    static BBox empty();
    static BBox full();

    void extend(const Point& point);
    void extend(const BBox& bbox);

    Vector diagonal() const {
        return static_cast<Vector>(max - min);
    }

    float area() const {
        Vector d = diagonal();
        return 2 * (d.x * d.y + d.y * d.z + d.z * d.x);
    }

    std::pair<float, float> intersect(const Ray& ray) const;

    bool isUnbound() const;
    Axis biggestDimensionAxis() const;
    bool isEmpty() const;
    Point center() const;
    std::vector<Point> getCorners() const;
    friend std::ostream& operator<<(std::ostream& os, const BBox& box);
private:
//     bool isEmpty = false;
//     bool isFull = false;
    bool isFull() const;
    void ensureThickness();
};

}

#endif