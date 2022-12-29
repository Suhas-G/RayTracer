#include <rt/materials/lambertian.h>
#include <rt/textures/texture.h>
#include <core/scalar.h>

namespace rt {

LambertianMaterial::LambertianMaterial(Texture* emission, Texture* diffuse)
: emission(emission), diffuse(diffuse)
{
    /* TODO */
}

RGBColor LambertianMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(outDir);
    return diffuse->getColor(texPoint) * rt::dot(normal, inDir) / rt::pi;
    
}

RGBColor LambertianMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(normal);
    CG_UNUSED(outDir);

    /* TODO */
    return emission->getColor(texPoint) / rt::pi;
}

Material::SampleReflectance LambertianMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
	UNREACHABLE;
}

Material::Sampling LambertianMaterial::useSampling() const {
	return SAMPLING_NOT_NEEDED;
}

}