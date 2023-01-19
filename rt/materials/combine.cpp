#include <tuple>
#include <rt/materials/combine.h>

namespace rt {

CombineMaterial::CombineMaterial()
{
    /* TODO */
}

void CombineMaterial::add(Material* m, float w) {
    materials.push_back(std::make_tuple(m, w));
}

RGBColor CombineMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    RGBColor color = RGBColor::rep(0.0f);
    for (auto &item: materials) {
        Material* m = std::get<0>(item);
        float weight = std::get<1>(item);
        color = color + (weight * m->getReflectance(texPoint, normal, outDir, inDir));
    }
    return color;
}

RGBColor CombineMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    RGBColor color = RGBColor::rep(0.0f);
    for (auto &item: materials) {
        Material* m = std::get<0>(item);
        float weight = std::get<1>(item);
        color = color + (weight * m->getEmission(texPoint, normal, outDir));
    }
    return color;
}

Material::SampleReflectance CombineMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    /* TODO */ NOT_IMPLEMENTED;
}

Material::Sampling CombineMaterial::useSampling() const {
    /* TODO */ NOT_IMPLEMENTED;
}

}