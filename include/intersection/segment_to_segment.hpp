#ifndef SEGMENT_TO_SEGMENT_HPP
#define SEGMENT_TO_SEGMENT_HPP

#include "common/cmp.hpp"
#include "intersection/point_to_segment.hpp"
#include "primitives/point.hpp"
#include "primitives/vector.hpp"

template <std::floating_point T>
bool check_segments_intersect_3d(const triangle::Point<T> &A, const triangle::Point<T> &B, const triangle::Point<T> &C,
                                        const triangle::Point<T> &D) {
    triangle::Vector<T> AB{A, B};
    triangle::Vector<T> CD{C, D};
    triangle::Vector<T> AC{A, C};

    triangle::Vector<T> n = vector_product(AB, CD);

    // Check that all 4 points are in the same plane
    if (!cmp::is_zero(scalar_product(n, AC)))
        return false;

    // Check via parameters
    T t_num = scalar_product(vector_product(AC, CD), n);
    T u_num = scalar_product(vector_product(AC, AB), n);
    T denom = scalar_product(n, n);

    if (cmp::is_zero(denom)) {
        // Collinear case
        return is_point_on_segment(A, B, C) || is_point_on_segment(A, B, D) ||
               is_point_on_segment(C, D, A) || is_point_on_segment(C, D, B);
    }

    T t = t_num / denom;
    T u = u_num / denom;

    return (t >= -cmp::float_eps && t <= 1.0 + cmp::float_eps && u >= -cmp::float_eps &&
            u <= 1.0 + cmp::float_eps);
}

template <std::floating_point T>
inline bool check_segments_intersect_2d(const triangle::Point<T> &a, const triangle::Point<T> &b, const triangle::Point<T> &c,
                                        const triangle::Point<T> &d) {
    auto orient_2d_simple = [](const triangle::Point<T> &p, const triangle::Point<T> &q, const triangle::Point<T> &r) -> double {
        return (q.get_x() - p.get_x()) * (r.get_y() - p.get_y()) -
               (q.get_y() - p.get_y()) * (r.get_x() - p.get_x());
    };

    T o1 = orient_2d_simple(a, b, c);
    T o2 = orient_2d_simple(a, b, d);
    T o3 = orient_2d_simple(c, d, a);
    T o4 = orient_2d_simple(c, d, b);

    // Common intersection (the segments intersect)
    if (((o1 > cmp::float_eps && o2 < -cmp::float_eps) ||
         (o1 < -cmp::float_eps && o2 > cmp::float_eps)) &&
        ((o3 > cmp::float_eps && o4 < -cmp::float_eps) ||
         (o3 < -cmp::float_eps && o4 > cmp::float_eps)))
        return true;

    // Collinear cases - the point lies on the segment
    auto on_segment = [](const triangle::Point<T> &p, const triangle::Point<T> &q, const triangle::Point<T> &r) -> bool {
        return (r.get_x() <= std::max(p.get_x(), q.get_x()) + cmp::float_eps &&
                r.get_x() >= std::min(p.get_x(), q.get_x()) - cmp::float_eps &&
                r.get_y() <= std::max(p.get_y(), q.get_y()) + cmp::float_eps &&
                r.get_y() >= std::min(p.get_y(), q.get_y()) - cmp::float_eps);
    };

    if (std::abs(o1) <= cmp::float_eps && on_segment(a, b, c))
        return true;
    if (std::abs(o2) <= cmp::float_eps && on_segment(a, b, d))
        return true;
    if (std::abs(o3) <= cmp::float_eps && on_segment(c, d, a))
        return true;
    if (std::abs(o4) <= cmp::float_eps && on_segment(c, d, b))
        return true;

    return false;
}

#endif
