#include <rt/materials/flatmaterial.h>
#include <rt/textures/texture.h>

namespace rt {

FlatMaterial::FlatMaterial(Texture* texture)
    : texture(texture)
{
}

RGBColor FlatMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    CG_UNUSED(inDir);
    return RGBColor::rep(0.0f);
}

RGBColor FlatMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    return texture->getColor(texPoint);
}

Material::SampleReflectance FlatMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    UNREACHABLE;
}

Material::Sampling FlatMaterial::useSampling() const {
    return Material::SAMPLING_NOT_NEEDED;
}

}