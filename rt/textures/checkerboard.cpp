#include <cmath>
#include <core/point.h>
#include <rt/textures/checkerboard.h>

namespace rt {

CheckerboardTexture::CheckerboardTexture(const RGBColor& white, const RGBColor& black)
: white(white), black(black)
{
    /* TODO */
}

RGBColor CheckerboardTexture::getColor(const Point& coord) {

    /* TODO */
    // bool xless = (coord.x - std::floor(coord.x)) <= this->edgeLength;
    // bool yless = (coord.y - std::floor(coord.y)) <= this->edgeLength;
    // bool zless = (coord.z - std::floor(coord.z)) <= this->edgeLength;

    // // || (xless && !yless && !zless) || (!xless && yless && !zless) || (!xless && !yless && zless)
    // if ((xless && yless && zless) || (xless && !yless && !zless) || (!xless && yless && !zless)) {
    //     return white;
    // } else {
    //     return black;
    // }
    if (((int)std::floor(coord.x * 0.5f) + (int)std::floor(coord.y * 0.5f) + (int)std::floor(coord.z * 0.5f)) % 2 == 0) {
        return black;
    } else {
        return white;
    }
}

RGBColor CheckerboardTexture::getColorDX(const Point& coord) {
    CG_UNUSED(coord);
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor CheckerboardTexture::getColorDY(const Point& coord) {
    CG_UNUSED(coord);
    /* TODO */ NOT_IMPLEMENTED;
}

}