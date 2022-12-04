#ifndef CG1RAYTRACER_MATRIX_HEADER
#define CG1RAYTRACER_MATRIX_HEADER

#include <core/homogeneouscoord.h>

namespace rt {

class Matrix {
public:
    Matrix() {}
    Matrix(const HomogeneousCoord& r1, const HomogeneousCoord& r2, const HomogeneousCoord& r3, const HomogeneousCoord& r4);

    HomogeneousCoord& operator[](int idx);
    HomogeneousCoord operator[](int idx) const;

    Matrix operator+(const Matrix& b) const;
    Matrix operator-(const Matrix& b) const;
    
    Matrix transpose() const;
    Matrix invert() const;

    bool operator==(const Matrix& b) const;
    bool operator!=(const Matrix& b) const;


    HomogeneousCoord operator*(const HomogeneousCoord& b) const;
    Vector operator*(const Vector& b) const;
    Point operator*(const Point& b) const;

    float det() const;

    static Matrix zero();
    static Matrix identity();

    static Matrix system(const Vector& e1, const Vector& e2, const Vector& e3);
};

Matrix product(const Matrix& a, const Matrix& b);
Matrix operator*(const Matrix& a, float scalar);
Matrix operator*(float scalar, const Matrix& a);

}

#endif