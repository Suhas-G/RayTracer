#include <rt/normalmappers/imagenormal.h>


namespace rt{
    ImageNormalMapper::ImageNormalMapper(ImageTexture* imgTexture)
    : NormalMapper(), imgTexture(imgTexture) {

    }

    Vector ImageNormalMapper::computePerturbation(const Intersection& hit, const Point& coords) const {
        RGBColor color = this->imgTexture->getColor(coords);
        Vector perturbation = Vector(color.r * 2 - 1, color.g * 2 - 1, color.b);
        return perturbation;
    }
}