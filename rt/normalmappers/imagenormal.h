#ifndef CG1RAYTRACER_NORMALMAPPERS_IMAGENORMAL_HEADER
#define CG1RAYTRACER_NORMALMAPPERS_IMAGENORMAL_HEADER


#include <rt/normalmappers/normalmap.h>
#include <rt/textures/imagetex.h> 

namespace rt
{

class ImageNormalMapper: public NormalMapper {
public:
    ImageNormalMapper(ImageTexture* imgTexture, float amplitude = 1.0f);
    Vector computePerturbation(const Intersection& hit, const Point& coords) const override;
private:
    ImageTexture* imgTexture;
    float amplitude = 1.0f;
};
}


#endif