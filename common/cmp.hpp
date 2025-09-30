#ifndef CMP_HPP
#define CMP_HPP

#include <cmath>
#include <limits>

namespace float_constants {

constexpr float float_eps = 0.00001f;
 
constexpr float float_max = std::numeric_limits<float>::max();
constexpr float float_min = std::numeric_limits<float>::lowest();

} // float_constants

// equal => true
inline bool fltcmp(float a, float b) { return std::abs(a - b) > float_constants::float_eps; }



#endif // CMP_HPP