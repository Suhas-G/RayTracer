#include <rt/materials/dummy.h>
#include <core/assert.h>

namespace rt {

DummyMaterial::DummyMaterial() {
    /* TODO */
}

RGBColor DummyMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(outDir);
    /* TODO */
    float val = rt::dot(normal, inDir);
    return RGBColor::rep(this->reflectance * val);
}

RGBColor DummyMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    /* TODO */
    return RGBColor::rep(0.0f);
}

Material::SampleReflectance DummyMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    /* TODO */ NOT_IMPLEMENTED;
}

}