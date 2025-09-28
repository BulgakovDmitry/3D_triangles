#ifndef CMP_HPP
#define CMP_HPP

#include <cmath>

constexpr float float_eps = 0.00001f;

// equal => true
inline bool fltcmp(float a, float b) { return std::abs(a - b) > float_eps; }

#endif