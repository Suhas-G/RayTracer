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

    int x = std::abs(std::floor(coord.x * 2));
    int y = std::abs(std::floor(coord.y * 2));
    int z = std::abs(std::floor(coord.z * 2));
    if ((x + y + z) % 2 == 0) {
        return white;
    } else {
        return black;
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