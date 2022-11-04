#include <core/color.h>
#include <core/scalar.h>
#include <core/homogeneouscoord.h>

namespace rt {

RGBColor::RGBColor(const HomogeneousCoord& coord)
{
/* TODO */ NOT_IMPLEMENTED;
}

RGBColor RGBColor::operator + (const RGBColor& c) const {
    return RGBColor(rt::clamp01(r + c.r), rt::clamp01(g + c.g), rt::clamp01(b + c.b));
}

RGBColor RGBColor::operator - (const RGBColor& c) const {
    return RGBColor(rt::clamp01(r - c.r), rt::clamp01(g - c.g), rt::clamp01(b - c.b));
}

RGBColor RGBColor::operator * (const RGBColor& c) const {
    return RGBColor(rt::clamp01(r * c.r), rt::clamp01(g * c.g), rt::clamp01(b * c.b));
}

bool RGBColor::operator == (const RGBColor& c) const {
    return (
        std::abs(r - c.r) < rt::epsilon &&
        std::abs(g - c.g) < rt::epsilon &&
        std::abs(b - c.b) < rt::epsilon
    );
}

bool RGBColor::operator != (const RGBColor& b) const {
    return !(*this == b);
}

RGBColor RGBColor::clamp() const {
    return RGBColor(rt::clamp01(r), rt::clamp01(g), rt::clamp01(b));
}

RGBColor RGBColor::gamma(float gam) const {
    /* TODO */ NOT_IMPLEMENTED;
}

float RGBColor::luminance() const {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor operator * (float scalar, const RGBColor& c) {
    return RGBColor(rt::clamp01(scalar * c.r), rt::clamp01(scalar * c.g), rt::clamp01(scalar * c.b));
}

RGBColor operator * (const RGBColor& c, float scalar) {
    return scalar * c;
}

RGBColor operator / (const RGBColor& c, float scalar) {
    return (1/scalar) * c;
}

}
