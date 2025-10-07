#ifndef TRIANGLE_TO_TRIANGLE_2D_HPP
#define TRIANGLE_TO_TRIANGLE_2D_HPP

#include <cstddef>

#include "primitives/triangle.hpp"
#include "primitives/vector.hpp"

namespace intersection_2d {

enum class Sign {
    different,
    pozitive,
    negative,
    common_vertice_other_poz_or_neg,
    common_plane,
};

inline float orient_2d(const Point &a, const Point &b, const Point &c, const Vector &n) {
    return mixed_product(Vector(a, b), Vector(a, c), n);
}

Sign check_relative_positions_2d(const Point &p, const Point &A, const Point &B, const Point &C,
                                 const Vector &n) {
    double s1 = orient_2d(A, B, p, n);
    double s2 = orient_2d(B, C, p, n);
    double s3 = orient_2d(C, A, p, n);

    Sign sign = Sign::different;

    if (s1 >= -cmp::float_eps && s2 >= -cmp::float_eps && s3 >= -cmp::float_eps)
        sign = Sign::pozitive;

    if (s1 <= cmp::float_eps && s2 <= cmp::float_eps && s3 <= cmp::float_eps)
        sign = Sign::negative;

    return sign;
}

bool on_segment_in_plane(const Point &a, const Point &b, const Point &p, const Vector &n) {
    if (std::abs(orient_2d(a, b, p, n)) > cmp::float_eps)
        return false;

    Vector ab = Vector(a, b);
    Vector ap = Vector(a, p);

    double t = scalar_product(ap, ab);
    double L2 = scalar_product(ab, ab);

    if (t < -cmp::float_eps)
        return false;
    if (t > L2 + cmp::float_eps)
        return false;

    return true;
}

bool check_segment_intersect_2d(const Point &a, const Point &b, const Point &c, const Point &d,
                                const Vector &n) {
    double o1 = orient_2d(a, b, c, n);
    double o2 = orient_2d(a, b, d, n);
    double o3 = orient_2d(c, d, a, n);
    double o4 = orient_2d(c, d, b, n);

    bool straddle1 = (o1 > cmp::float_eps && o2 < -cmp::float_eps) ||
                     (o1 < -cmp::float_eps && o2 > cmp::float_eps);
    bool straddle2 = (o3 > cmp::float_eps && o4 < -cmp::float_eps) ||
                     (o3 < -cmp::float_eps && o4 > cmp::float_eps);
    if (straddle1 && straddle2)
        return true;

    if (std::abs(o1) <= cmp::float_eps && on_segment_in_plane(a, b, c, n))
        return true;
    if (std::abs(o2) <= cmp::float_eps && on_segment_in_plane(a, b, d, n))
        return true;
    if (std::abs(o3) <= cmp::float_eps && on_segment_in_plane(c, d, a, n))
        return true;
    if (std::abs(o4) <= cmp::float_eps && on_segment_in_plane(c, d, b, n))
        return true;

    return false;
}

inline bool intersect_2d(const Triangle &first, const Triangle &second) {
    const auto &A = first.get_vertices();
    const auto &B = second.get_vertices();

    Vector n = vector_product(Vector(A[0], A[1]), Vector(A[0], A[2]));

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

} // namespace intersection_2d

#endif
