#ifndef CMP_HPP
#define CMP_HPP

#include <algorithm>
#include <cmath>
#include <type_traits>

namespace cmp {
constexpr float float_eps = 1e-16f;

// equal => true
inline bool     fltcmp(float a, float b) { return std::abs(a - b) > float_eps; }

template <typename T>
    requires std::is_floating_point_v<T>
struct precision final {
    static constexpr T epsilon = 1e-16f;
};

template <typename T>
constexpr bool are_equal(T first, T second, T zero_diff = precision<T>::epsilon) {
    return (std::abs(first - second) < zero_diff);
}

template <typename T> constexpr bool is_zero(T value, T zero_diff = precision<T>::epsilon) {
    return std::abs(value) < zero_diff;
}

template <typename T> constexpr bool pozitive(T value, T pozitive_zero = precision<T>::epsilon) {
    return value >= pozitive_zero;
}

template <typename T>
constexpr bool non_pozitive(T value, T pozitive_zero = precision<T>::epsilon) {
    return value < pozitive_zero;
}

template <typename T> constexpr bool negative(T value, T negative_zero = -precision<T>::epsilon) {
    return value <= negative_zero;
}

template <typename T>
constexpr bool non_negative(T value, T negative_zero = -precision<T>::epsilon) {
    return value > negative_zero;
}

} // namespace cmp

#endif
