
#include <core/interpolate.h>
#include <core/point.h>
#include <core/homogeneouscoord.h>

namespace rt {

Point lerp(const Point& px0, const Point& px1, float xPoint) { return Point(lerp(HomogeneousCoord(px0), HomogeneousCoord(px1), xPoint)); }
Point lerpbar(const Point& px0, const Point& px1, const Point& px2, float xWeight, float yWeight) { return Point(lerpbar(HomogeneousCoord(px0), HomogeneousCoord(px1), HomogeneousCoord(px2), xWeight, yWeight)); }
Point lerp2d(const Point& px0y0, const Point& px1y0, const Point& px0y1, const Point& px1y1, float xPoint, float yPoint) {
    return Point(lerp2d(HomogeneousCoord(px0y0), HomogeneousCoord(px1y0),  HomogeneousCoord(px0y1), HomogeneousCoord(px1y1), xPoint, yPoint));
}
Point lerp3d(const Point& px0y0z0, const Point& px1y0z0, const Point& px0y1z0, const Point& px1y1z0,
    const Point& px0y0z1, const Point& px1y0z1, const Point& px0y1z1, const Point& px1y1z1,
    float xPoint, float yPoint, float zPoint) {
    return Point(lerp3d(HomogeneousCoord(px0y0z0), HomogeneousCoord(px1y0z0), HomogeneousCoord(px0y1z0), HomogeneousCoord(px1y1z0),
                        HomogeneousCoord(px0y0z1), HomogeneousCoord(px1y0z1), HomogeneousCoord(px0y1z1), HomogeneousCoord(px1y1z1),
                        xPoint, yPoint, zPoint));
}

}