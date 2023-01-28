#include <tuple>
#include <rt/materials/combine.h>
#include <core/random.h>
#include <cmath>

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
        if (m->useSampling() == Material::SAMPLING_NOT_NEEDED) {
            color = color + (weight * m->getReflectance(texPoint, normal, outDir, inDir));
        }
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
    /* TODO */
    std::vector<SampleReflectance> reflectances;
    for (auto &item: materials) {
        Material* m = std::get<0>(item);
        if (m->useSampling() == rt::Material::SAMPLING_ALL) {
            SampleReflectance reflectance = m->getSampleReflectance(texPoint, normal, outDir);
            reflectances.push_back(reflectance);
        }
    }
    int index = std::max(static_cast<int>(std::floor(rt::random() * reflectances.size())), static_cast<int>(reflectances.size() - 1));
    return reflectances[index];
}

Material::Sampling CombineMaterial::useSampling() const {
    /* TODO */
    return Material::SAMPLING_SECONDARY;
}

}