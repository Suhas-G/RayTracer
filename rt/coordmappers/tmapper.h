#ifndef CG1RAYTRACER_COORDMAPPERS_TMAPPER_HEADER
#define CG1RAYTRACER_COORDMAPPERS_TMAPPER_HEADER

#include <rt/coordmappers/coordmapper.h>

namespace rt {

class TriangleMapper : public CoordMapper {
public:
    TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2);
    TriangleMapper(Point ntv[3]);
    virtual Point getCoords(const Intersection& hit) const;
private:
    Point v0;
    Point v1;
    Point v2;
};

}



#endif