#include <core/homogeneouscoord.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/assert.h>
#include <core/scalar.h>

namespace rt {

HomogeneousCoord::HomogeneousCoord(float x, float y, float z, float w) {
    /* TODO */
}

HomogeneousCoord::HomogeneousCoord(const Point& p) {
    /* TODO */ NOT_IMPLEMENTED;
}

HomogeneousCoord::HomogeneousCoord(const Vector& vec) {
    /* TODO */ NOT_IMPLEMENTED;
}

float& HomogeneousCoord::operator [] (int idx) {
    /* TODO */ NOT_IMPLEMENTED;
}

float HomogeneousCoord::operator [] (int idx) const {
    /* TODO */ NOT_IMPLEMENTED;
}

HomogeneousCoord HomogeneousCoord::operator + (const HomogeneousCoord& b) const {
    /* TODO */ NOT_IMPLEMENTED;
}

HomogeneousCoord HomogeneousCoord::operator - (const HomogeneousCoord& b) const {
    /* TODO */ NOT_IMPLEMENTED;
}

HomogeneousCoord HomogeneousCoord::operator * (const HomogeneousCoord& b) const {
    /* TODO */ NOT_IMPLEMENTED;
}

HomogeneousCoord HomogeneousCoord::operator / (const HomogeneousCoord& b) const {
    /* TODO */ NOT_IMPLEMENTED;
}

HomogeneousCoord operator * (float scalar, const HomogeneousCoord& b) {
    /* TODO */ NOT_IMPLEMENTED;
}

HomogeneousCoord operator * (const HomogeneousCoord& a, float scalar) {
    /* TODO */ NOT_IMPLEMENTED;
}

HomogeneousCoord operator / (const HomogeneousCoord& a, float scalar) {
    /* TODO */ NOT_IMPLEMENTED;
}

float dot(const HomogeneousCoord& a, const HomogeneousCoord& b) {
    /* TODO */ NOT_IMPLEMENTED;
}

HomogeneousCoord HomogeneousCoord::operator - () const {
    /* TODO */ NOT_IMPLEMENTED;
}

bool HomogeneousCoord::operator == (const HomogeneousCoord& b) const {
    /* TODO */ NOT_IMPLEMENTED;
}

bool HomogeneousCoord::operator != (const HomogeneousCoord& b) const {
    /* TODO */ NOT_IMPLEMENTED;
}

HomogeneousCoord min(const HomogeneousCoord& a, const HomogeneousCoord& b) {
    /* TODO */ NOT_IMPLEMENTED;
}

HomogeneousCoord max(const HomogeneousCoord& a, const HomogeneousCoord& b) {
    /* TODO */ NOT_IMPLEMENTED;
}

}