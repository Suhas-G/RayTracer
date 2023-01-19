#include <rt/textures/imagetex.h>
#include <core/interpolate.h>
#include <core/point.h>

namespace rt {

ImageTexture::ImageTexture()
{
    /* TODO */
}

ImageTexture::ImageTexture(const std::string& filename, BorderHandlingType bh, InterpolationType i)
: bh(bh), interpolation(i)
{
    this->image = Image();
    this->image.readPNG(filename); 
}

ImageTexture::ImageTexture(const Image& image, BorderHandlingType bh, InterpolationType i)
: image(image), bh(bh), interpolation(i)
{
    /* TODO */
}

std::tuple<float, float> ImageTexture::normalizeCoords(float x, float y) {
    switch (bh)
    {
    case ImageTexture::BorderHandlingType::REPEAT:
        return std::make_tuple(x - std::floor(x), y - std::floor(y));
        break;
    case ImageTexture::BorderHandlingType::MIRROR:
    {
        float fx = std::floor(x);
        float fy = std::floor(y);
        float tu = x - fx;
        float tv = y - fy;
        if (std::abs(static_cast<int>(fx)) % 2 == 1) {
            tu = 1 - tu;
        }
        if (std::abs(static_cast<int>(fy)) % 2 == 1) {
            tv = 1 - tv;
        }
        return std::make_tuple(tu, tv);
    }
        break;
    case ImageTexture::BorderHandlingType::CLAMP:
        return std::make_tuple(rt::clamp01(x), rt::clamp01(y));
        break;
    default:
        break;
    }
    return std::make_tuple(x, y);
}

RGBColor ImageTexture::interpolate(float tu, float tv) {
    float u, v;
    uint lu, lv;
    switch (interpolation)
    {
    case ImageTexture::InterpolationType::NEAREST:
    {
        u = tu * this->image.width();
        v = tv * this->image.height();
        lu = std::min(static_cast<uint>(std::floor(u)), this->image.width() - 1);
        lv = std::min(static_cast<uint>(std::floor(v)), this->image.height() - 1);
        return this->image(lu, lv);
        break;
    }
    case ImageTexture::InterpolationType::BILINEAR:
    {
        u = tu * this->image.width() - 1;
        v = tv * this->image.height() - 1;
        float fu = u - std::floor(u);
        float fv = v - std::floor(v);
        lu = std::min(static_cast<uint>(std::floor(u)), this->image.width() - 1);
        lv = std::min(static_cast<uint>(std::floor(v)), this->image.height() - 1);
        uint ludx = std::min(lu + 1, this->image.width() - 1);
        uint lvdy = std::min(lv + 1, this->image.height() - 1);
        // FIXME: Use lerp later
        return (((1 - fu) * (1 - fv) * this->image(lu, lv)) + ((1 - fu) * fv * this->image(lu, lvdy)) + 
                (fu * (1 - fv) * this->image(ludx, lv)) + (fu * fv * this->image(ludx, lvdy)));
    }
        break;
    default:
        break;
    }
    return RGBColor::rep(0.0f);
}

RGBColor ImageTexture::getColor(const Point& coord) {
    /* TODO */
    float nx, ny;
    std::tie(nx, ny) = normalizeCoords(coord.x, coord.y);
    RGBColor color = interpolate(nx, ny);
    
    return color;
}

RGBColor ImageTexture::getColorDX(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor ImageTexture::getColorDY(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

}