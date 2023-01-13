#include <rt/textures/constant.h>

namespace rt {

ConstantTexture::ConstantTexture()
: color(RGBColor(1.0f, 0.0f, 1.0f)) {
    /* TODO */
    // Magenta color to debug if its not initialised
}

ConstantTexture::ConstantTexture(const RGBColor& color)
: color(color)
{
    /* TODO */
}

RGBColor ConstantTexture::getColor(const Point& coord) {
    CG_UNUSED(coord);
    /* TODO */
    return this->color;
}

RGBColor ConstantTexture::getColorDX(const Point& coord) {
    CG_UNUSED(coord);
    /* TODO */
    return RGBColor::rep(0.0f);
}

RGBColor ConstantTexture::getColorDY(const Point& coord) {
    CG_UNUSED(coord);
    /* TODO */
    return RGBColor::rep(0.0f);
}

}