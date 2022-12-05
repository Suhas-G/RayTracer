#include <core/matrix.h>
#include <core/assert.h>

#include <core/vector.h>
#include <core/point.h>
#include <core/scalar.h>

namespace rt {

Matrix::Matrix(const HomogeneousCoord& r1, const HomogeneousCoord& r2, const HomogeneousCoord& r3, const HomogeneousCoord& r4) {
    /* TODO */
    // Assuming each one is a row
    rows[0] = r1;
    rows[1] = r2;
    rows[2] = r3;
    rows[3] = r4;
}

HomogeneousCoord& Matrix::operator[](int idx) {
    return rows[idx];
}

HomogeneousCoord Matrix::operator[](int idx) const {
    return rows[idx];
}

Matrix Matrix::operator+(const Matrix& b) const {
    // TODO: Using loop is simple, but is this faster?
    return Matrix(this->rows[0] + b.rows[0], 
                  this->rows[1] + b.rows[1], 
                  this->rows[2] + b.rows[2],
                  this->rows[3] + b.rows[3]);
}

Matrix Matrix::operator-(const Matrix& b) const {
    return Matrix(this->rows[0] - b.rows[0], 
                  this->rows[1] - b.rows[1], 
                  this->rows[2] - b.rows[2],
                  this->rows[3] - b.rows[3]);
}

Matrix Matrix::transpose() const {
    Matrix m;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = (*this)[j][i];
        }
    }
    return m;
}

Matrix Matrix::invert() const {
    Matrix result;
    const Matrix& m = *this;

    // Taken and modified from http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    result[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
    result[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
    result[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
    result[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];

    float det = m[0][0] * result[0][0] + m[0][1] * result[1][0] + m[0][2] * result[2][0] + m[0][3] * result[3][0];
    if (det == 0)
        return Matrix::zero();

    result[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
    result[1][1] =  m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
    result[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
    result[3][1] =  m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
    result[0][2] =  m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
    result[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
    result[2][2] =  m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
    result[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
    result[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
    result[1][3] =  m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
    result[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
    result[3][3] =  m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

    result = result*(1.0f/det);
    return result;
}

bool Matrix::operator==(const Matrix& b) const {
    return (this->rows[0] == b.rows[0] && this->rows[1] == b.rows[1] && this->rows[2] == b.rows[2] && this->rows[3] == b.rows[3]);
}

bool Matrix::operator!=(const Matrix& b) const {
    return !(*this == b);
}

Matrix product(const Matrix& a, const Matrix& b) {
    /* TODO */
    Matrix result = Matrix::rep(HomogeneousCoord::rep(0.0f));
    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 4; k++) {
            for (int j = 0; j < 4; j++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

Matrix operator*(const Matrix& a, float scalar) {
    return Matrix(scalar * a[0], scalar * a[1], scalar * a[2], scalar * a[3]);
}

Matrix operator*(float scalar, const Matrix& a) {
    return a * scalar;
}

HomogeneousCoord Matrix::operator*(const HomogeneousCoord& b) const {
    float c1 = rt::dot(rows[0], b);
    float c2 = rt::dot(rows[1], b);
    float c3 = rt::dot(rows[2], b);
    float c4 = rt::dot(rows[3], b);
    return HomogeneousCoord(c1, c2, c3, c4);
}

Vector Matrix::operator*(const Vector& b) const {
    // Measure: Which is faster? Does it matter?
    // return static_cast<Vector>(*this * static_cast<HomogeneousCoord>(b));
    Vector v;
    v.x = rt::dot(rows[0], b);
    v.y = rt::dot(rows[1], b);
    v.z = rt::dot(rows[2], b);
    float w = rt::dot(rows[3], b);
    rt_release_assert(std::abs(w) < rt::epsilon);
    return v;
}

Point Matrix::operator*(const Point& b) const {
    // Measure: Which is faster? Does it matter?
    // return static_cast<Point>(*this * static_cast<HomogeneousCoord>(b));
    Point p;
    p.x = rt::dot(rows[0], b);
    p.y = rt::dot(rows[1], b);
    p.z = rt::dot(rows[2], b);
    float w = rt::dot(rows[3], b);
    rt_release_assert(std::abs(w - 1.0f) < rt::epsilon);
    return p;
}

float Matrix::det() const {
    const Matrix& m = *this;
    return (
        m[0][0] * (m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + 
                m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + 
                m[1][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3])) +
        m[0][1] * (m[1][0] * (-m[2][2] * m[3][3] + m[2][3] * m[3][2]) + 
                m[1][2] * (-m[2][3] * m[3][0] + m[2][0] * m[3][3]) + 
                m[1][3] * (-m[2][1] * m[3][2] + m[2][2] * m[3][0])) + 
        m[0][2] * (m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + 
                m[1][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) +
                m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0])) + 
        m[0][3] * (m[1][0] * (-m[2][1] * m[3][2] + m[2][2] * m[3][1]) + 
                m[1][1] * (-m[2][2] * m[3][0] + m[2][0] * m[3][2]) + 
                m[1][2] * (-m[2][0] * m[3][1] + m[2][1] * m[3][0]))
    );
}

Matrix Matrix::zero() {
    return Matrix::rep(HomogeneousCoord::rep(0));
}

Matrix Matrix::identity() {
    return Matrix(
        HomogeneousCoord(1, 0, 0, 0),
        HomogeneousCoord(0, 1, 0, 0),
        HomogeneousCoord(0, 0, 1, 0),
        HomogeneousCoord(0, 0, 0, 1)
    );
}

Matrix Matrix::system(const Vector& e1, const Vector& e2, const Vector& e3) {
    /* TODO */
    // Assuming the basis vectors must become columns
    return Matrix(HomogeneousCoord(e1), HomogeneousCoord(e2), HomogeneousCoord(e3), HomogeneousCoord(0, 0, 0, 1.0f)).transpose();
}

}