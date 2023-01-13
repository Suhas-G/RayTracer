#include <rt/materials/phong.h>
#include <rt/textures/texture.h>
#include <cmath>
#include <core/scalar.h>

namespace rt {

PhongMaterial::PhongMaterial(Texture* specular, float exponent)
: specular(specular), exponent(exponent)
{
    /* TODO */
}

RGBColor PhongMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(outDir);

    /* TODO */
    float inDotNormal = rt::dot(inDir, normal);
    Vector RI = (-inDir + 2 * (inDotNormal) * normal).normalize();
    // Vector h = (inDir + outDir).normalize();
    // FIXME: Not divided by inDotNormal as shown in slides as produces artifacts
    return specular->getColor(texPoint) * (std::pow(rt::clamp01(rt::dot(RI, outDir)), this->exponent));
}

RGBColor PhongMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);

    return RGBColor::rep(0.0f);
}

Material::SampleReflectance PhongMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);

	UNREACHABLE;
}

Material::Sampling PhongMaterial::useSampling() const {
	/* TODO */ NOT_IMPLEMENTED;
}

}