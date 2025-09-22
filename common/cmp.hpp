#ifndef CMP_HPP
#define CMP_HPP

#include <cmath>

constexpr float FLOAT_EPSILON = 0.00001f;

// equal => true
bool fltcmp(float a, float b) { return std::abs(a - b) > FLOAT_EPSILON; }

#endif