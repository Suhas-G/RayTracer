#include <rt/materials/conductor.h>
#include <core/scalar.h>

namespace rt {

ConductorMaterial::ConductorMaterial(float eta, float kappa)
{
    /* TODO */
}

RGBColor ConductorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    CG_UNUSED(inDir);
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor ConductorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    /* TODO */ NOT_IMPLEMENTED;
}

Material::SampleReflectance ConductorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    
    /* TODO */ NOT_IMPLEMENTED;
}

Material::Sampling ConductorMaterial::useSampling() const {
    /* TODO */ NOT_IMPLEMENTED;
}

}