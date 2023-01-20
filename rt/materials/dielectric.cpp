#include <rt/materials/dielectric.h>

namespace rt {

DielectricMaterial::DielectricMaterial(float eta)
{
    /* TODO */
}

RGBColor DielectricMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    CG_UNUSED(inDir);
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor DielectricMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    /* TODO */ NOT_IMPLEMENTED;
}

Material::SampleReflectance DielectricMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);

    /* TODO */ NOT_IMPLEMENTED;
}

Material::Sampling DielectricMaterial::useSampling() const {
    /* TODO */ NOT_IMPLEMENTED;
}

}