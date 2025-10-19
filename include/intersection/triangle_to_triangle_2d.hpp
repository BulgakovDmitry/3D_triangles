#ifndef TRIANGLE_TO_TRIANGLE_2D_HPP
#define TRIANGLE_TO_TRIANGLE_2D_HPP

#include <cstddef>

#include "primitives/triangle.hpp"
#include "primitives/vector.hpp"

namespace triangle {

enum class Sign {
    different,
    pozitive,
    negative,
    common_vertice_other_poz_or_neg,
    common_plane,
};

template <std::floating_point T>
T orient_2d(const Point<T> &a, const Point<T> &b, const Point<T> &c,
            const Vector<T> &n) {
    return mixed_product(Vector(a, b), Vector(a, c), n);
}

template <std::floating_point T>
Sign check_relative_positions_2d(const Point<T> &p, const Point<T> &A,
                                 const Point<T> &B, const Point<T> &C,
                                 const Vector<T> &n) {
    T s1 = orient_2d(A, B, p, n);
    T s2 = orient_2d(B, C, p, n);
    T s3 = orient_2d(C, A, p, n);

    Sign sign = Sign::different;

    if (s1 >= -cmp::precision<T>::epsilon && s2 >= -cmp::precision<T>::epsilon &&
        s3 >= -cmp::precision<T>::epsilon)
        sign = Sign::pozitive;

    if (s1 <= cmp::precision<T>::epsilon && s2 <= cmp::precision<T>::epsilon &&
        s3 <= cmp::precision<T>::epsilon)
        sign = Sign::negative;

    return sign;
}

template <std::floating_point T>
bool on_segment_in_plane(const Point<T> &a, const Point<T> &b,
                         const Point<T> &p, const Vector<T> &n) {
    if (std::abs(orient_2d(a, b, p, n)) > cmp::precision<T>::epsilon)
        return false;

    Vector ab = Vector(a, b);
    Vector ap = Vector(a, p);

    auto t = scalar_product(ap, ab);
    auto L2 = scalar_product(ab, ab);

    if (t < -cmp::precision<T>::epsilon)
        return false;
    if (t > L2 + cmp::precision<T>::epsilon)
        return false;

    return true;
}

template <std::floating_point T>
bool check_segment_intersect_2d(const Point<T> &a, const Point<T> &b,
                                const Point<T> &c, const Point<T> &d,
                                const Vector<T> &n) {
    T o1 = orient_2d(a, b, c, n);
    T o2 = orient_2d(a, b, d, n);
    T o3 = orient_2d(c, d, a, n);
    T o4 = orient_2d(c, d, b, n);

    bool straddle1 = (o1 > cmp::precision<T>::epsilon && o2 < -cmp::precision<T>::epsilon) ||
                     (o1 < -cmp::precision<T>::epsilon && o2 > cmp::precision<T>::epsilon);
    bool straddle2 = (o3 > cmp::precision<T>::epsilon && o4 < -cmp::precision<T>::epsilon) ||
                     (o3 < -cmp::precision<T>::epsilon && o4 > cmp::precision<T>::epsilon);
    if (straddle1 && straddle2)
        return true;

    if (std::abs(o1) <= cmp::precision<T>::epsilon && on_segment_in_plane(a, b, c, n))
        return true;
    if (std::abs(o2) <= cmp::precision<T>::epsilon && on_segment_in_plane(a, b, d, n))
        return true;
    if (std::abs(o3) <= cmp::precision<T>::epsilon && on_segment_in_plane(c, d, a, n))
        return true;
    if (std::abs(o4) <= cmp::precision<T>::epsilon && on_segment_in_plane(c, d, b, n))
        return true;

    return false;
}

template <std::floating_point T>
bool intersect_2d(const Triangle<T> &first, const Triangle<T> &second) {
    const auto &A = first.get_vertices();
    const auto &B = second.get_vertices();

    Vector<T> n =
        vector_product(Vector(A[0], A[1]), Vector(A[0], A[2]));

    for (std::size_t i = 0; i < 3; ++i) {
        auto relative_positions_2d = check_relative_positions_2d(A[i], B[0], B[1], B[2], n);
        if (relative_positions_2d == Sign::pozitive || relative_positions_2d == Sign::negative)
            return true;
    }

    for (std::size_t j = 0; j < 3; ++j) {
        auto relative_positions_2d = check_relative_positions_2d(B[j], A[0], A[1], A[2], n);
        if (relative_positions_2d == Sign::pozitive || relative_positions_2d == Sign::negative)
            return true;
    }

    for (std::size_t i = 0; i < 3; ++i) {
        std::size_t inext = (i + 1) % 3;
        for (std::size_t j = 0; j < 3; ++j) {
            std::size_t jnext = (j + 1) % 3;
            if (check_segment_intersect_2d(A[i], A[inext], B[j], B[jnext], n))
                return true;
        }
    }

    return false;
}

} // namespace triangle

#endif
