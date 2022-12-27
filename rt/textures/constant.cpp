#include <rt/textures/constant.h>

namespace rt {

ConstantTexture::ConstantTexture()
{
    /* TODO */
}

ConstantTexture::ConstantTexture(const RGBColor& color)
{
    CG_UNUSED(color);
    /* TODO */
}

RGBColor ConstantTexture::getColor(const Point& coord) {
    CG_UNUSED(coord);
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor ConstantTexture::getColorDX(const Point& coord) {
    CG_UNUSED(coord);
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor ConstantTexture::getColorDY(const Point& coord) {
    CG_UNUSED(coord);
    /* TODO */ NOT_IMPLEMENTED;
}

}