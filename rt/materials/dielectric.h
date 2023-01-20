#ifndef CG1RAYTRACER_MATERIALS_DIELECTRIC_HEADER
#define CG1RAYTRACER_MATERIALS_DIELECTRIC_HEADER

#include <rt/materials/material.h>

namespace rt {

/// A fancy perfect glass (eta = ~1.55) or dielectric in general
class DielectricMaterial : public Material {
public:
    DielectricMaterial(float eta);
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual Sampling useSampling() const;
};

}

#endif