#ifndef CG1RAYTRACER_TEXTURES_IMAGETEX_HEADER
#define CG1RAYTRACER_TEXTURES_IMAGETEX_HEADER

#include <tuple>
#include <core/image.h>
#include <rt/textures/texture.h>

namespace rt {

class ImageTexture : public Texture {
public:
    enum BorderHandlingType {
        CLAMP,
        MIRROR,
        REPEAT
    };

    enum InterpolationType {
        NEAREST,
        BILINEAR
    };

    ImageTexture();
    ImageTexture(const Image& image, BorderHandlingType bh=REPEAT, InterpolationType i=BILINEAR);
    ImageTexture(const std::string& filename, BorderHandlingType bh=REPEAT, InterpolationType i=BILINEAR);
    virtual RGBColor getColor(const Point& coord);
    virtual RGBColor getColorDX(const Point& coord);
    virtual RGBColor getColorDY(const Point& coord);
    
private:
    Image image;
    BorderHandlingType bh;
    InterpolationType interpolation;

    std::tuple<float, float> normalizeCoords(float x, float y);
    RGBColor interpolate(float tu, float tv);
};

}

#endif