#include <rt/materials/phong.h>

namespace rt {

PhongMaterial::PhongMaterial(Texture* specular, float exponent)
{
    /* TODO */
}

RGBColor PhongMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    CG_UNUSED(inDir);

    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor PhongMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);

    /* TODO */ NOT_IMPLEMENTED;
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