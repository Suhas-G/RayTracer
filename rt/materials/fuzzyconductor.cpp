#include <rt/materials/fuzzyconductor.h>

namespace rt {

FuzzyConductorMaterial::FuzzyConductorMaterial(float eta, float kappa, float fuzzyangle)
{
    /* TODO */
}

RGBColor FuzzyConductorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    CG_UNUSED(inDir);
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor FuzzyConductorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    /* TODO */ NOT_IMPLEMENTED;
}

Material::SampleReflectance FuzzyConductorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);

    /* TODO */ NOT_IMPLEMENTED;
}

Material::Sampling FuzzyConductorMaterial::useSampling() const {
    /* TODO */ NOT_IMPLEMENTED;
}

}
