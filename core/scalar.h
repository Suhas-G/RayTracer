#ifndef CG1RAYTRACER_SCALAR_HEADER
#define CG1RAYTRACER_SCALAR_HEADER

#include <float.h>
#include <algorithm>
#include <cmath>

namespace rt {

using std::min;
using std::max;
//template <typename T>
//T min(T a, T b) { return a<b?a:b; }

//template <typename T>
//T max(T a, T b) { return a>b?a:b; }

// Computes the minimum of 3 values of any type
template <typename T>
T min(T a, T b, T c) { return min(min(a,b),c); }

// Computes the maximum of 3 values of any type
template <typename T>
T max(T a, T b, T c) { return max(max(a,b),c); }

// Computes a square of
template <typename T>
T sqr(const T& a) { return a*a; }

// Returns the fractional part of a number. For negative numbers it returns the negative fractional
float fractional(float f);

// Returns the difference between the given value and the nearest lower integer. Always in range [0, 1)
float absfractional(float f);

// The epsilon value adjustment for precision errors.
static const float epsilon = 0.000001f;

// The mathematical pi constant
static const float pi = 3.141592f;

static const float OFFSET_MULTIPLIER = 100.0f;

typedef unsigned int uint;

float clamp01(const float &x);

inline bool approxEquals(float v1, float v2) {return std::abs(v1 - v2) < rt::epsilon;}

inline float sqr(float x) { return x * x;}


}

#endif