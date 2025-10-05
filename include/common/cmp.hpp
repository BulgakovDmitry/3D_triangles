#ifndef CMP_HPP
#define CMP_HPP

#include <algorithm>
#include <cmath>
#include <limits>
#include <type_traits>

namespace cmp {
inline constexpr float float_eps = 1e-16f;

// equal => true
inline bool fltcmp(float a, float b) { return std::abs(a - b) > float_eps; }

template <std::floating_point T> struct precision final {
    static constexpr T epsilon = 1e-16f;
};

template <typename T>
inline constexpr bool are_equal(T first, T second, T zero_diff = precision<T>::epsilon) {
    return (std::abs(first - second) < zero_diff);
}

template <typename T>
inline constexpr bool greater(T first, T second, T zero_diff = precision<T>::epsilon) {
    return (first - second) > zero_diff;
}

template <typename T> 
inline constexpr bool lower(T first, T second, T zero_diff = precision<T>::epsilon) {
    return (second - first) > zero_diff;
}

template <typename T> 
inline constexpr bool is_zero(T value, T zero_diff = precision<T>::epsilon) {
    return std::abs(value) < zero_diff;
}

template <typename T> 
inline constexpr bool pozitive(T value, T pozitive_zero = precision<T>::epsilon) {
    return value >= pozitive_zero;
}

template <typename T>
inline constexpr bool non_pozitive(T value, T pozitive_zero = precision<T>::epsilon) {
    return value < pozitive_zero;
}

template <typename T> 
inline constexpr bool negative(T value, T negative_zero = -precision<T>::epsilon) {
    return value <= negative_zero;
}

template <typename T>
inline constexpr bool non_negative(T value, T negative_zero = -precision<T>::epsilon) {
    return value > negative_zero;
}

} // namespace cmp

#endif // CMP_HPP
