#ifndef CMP_HPP
#define CMP_HPP

#include <cmath>
#include <concepts>

namespace cmp {
inline constexpr float float_eps = 1e-5f;

// equal => true
inline bool fltcmp(float a, float b) { return std::abs(a - b) > float_eps; }

template <std::floating_point T> struct precision final {
    static constexpr T epsilon = 1e-5f;
};

template <std::floating_point T>
inline bool are_equal(T first, T second, T zero_diff = precision<T>::epsilon) {
    return (std::abs(first - second) < zero_diff);
}

template <std::floating_point T>
inline bool greater(T first, T second, T zero_diff = precision<T>::epsilon) {
    return (first - second) > zero_diff;
}

template <std::floating_point T>
inline bool lower(T first, T second, T zero_diff = precision<T>::epsilon) {
    return (second - first) > zero_diff;
}

template <std::floating_point T>
inline bool greater_or_equal(T first, T second, T zero_diff = precision<T>::epsilon) {
    return ((first - second) > zero_diff) || are_equal(first, second);
}

template <std::floating_point T>
inline bool lower_or_equal(T first, T second, T zero_diff = precision<T>::epsilon) {
    return ((second - first) > zero_diff) || are_equal(first, second);
}

template <std::floating_point T>
inline bool is_zero(T value, T zero_diff = precision<T>::epsilon) {
    return std::abs(value) < zero_diff;
}

template <std::floating_point T>
inline bool pozitive(T value, T pozitive_zero = precision<T>::epsilon) {
    return value >= pozitive_zero;
}

template <std::floating_point T>
inline bool non_pozitive(T value, T pozitive_zero = precision<T>::epsilon) {
    return value < pozitive_zero;
}

template <std::floating_point T>
inline bool negative(T value, T negative_zero = -precision<T>::epsilon) {
    return value <= negative_zero;
}

template <std::floating_point T>
inline bool non_negative(T value, T negative_zero = -precision<T>::epsilon) {
    return value > negative_zero;
}

} // namespace cmp

#endif // CMP_HPP
