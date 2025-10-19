#ifndef SEGMENT_TO_SEGMENT_HPP
#define SEGMENT_TO_SEGMENT_HPP

#include "common/cmp.hpp"
#include "intersection/point_to_segment.hpp"
#include "primitives/point.hpp"
#include "primitives/vector.hpp"

namespace triangle {

template <std::floating_point T>
bool check_segments_intersect_3d(const Point<T> &A, const Point<T> &B,
                                 const Point<T> &C, const Point<T> &D) {
    Vector<T> AB{A, B};
    Vector<T> CD{C, D};
    Vector<T> AC{A, C};

    Vector<T> n = vector_product(AB, CD);

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

    return (t >= -cmp::precision<T>::epsilon && t <= 1.0 + cmp::precision<T>::epsilon &&
            u >= -cmp::precision<T>::epsilon && u <= 1.0 + cmp::precision<T>::epsilon);
}

template <std::floating_point T>
inline bool check_segments_intersect_2d(const Point<T> &a, const Point<T> &b,
                                        const Point<T> &c, const Point<T> &d) {
    auto orient_2d_simple = [](const Point<T> &p, const Point<T> &q,
                               const Point<T> &r) -> double {
        return (q.x_ - p.x_) * (r.y_ - p.y_) -
               (q.y_ - p.y_) * (r.x_ - p.x_);
    };

    T o1 = orient_2d_simple(a, b, c);
    T o2 = orient_2d_simple(a, b, d);
    T o3 = orient_2d_simple(c, d, a);
    T o4 = orient_2d_simple(c, d, b);

    // Common intersection (the segments intersect)
    if (((o1 > cmp::precision<T>::epsilon && o2 < -cmp::precision<T>::epsilon) ||
         (o1 < -cmp::precision<T>::epsilon && o2 > cmp::precision<T>::epsilon)) &&
        ((o3 > cmp::precision<T>::epsilon && o4 < -cmp::precision<T>::epsilon) ||
         (o3 < -cmp::precision<T>::epsilon && o4 > cmp::precision<T>::epsilon)))
        return true;

    // Collinear cases - the point lies on the segment
    auto on_segment = [](const Point<T> &p, const Point<T> &q,
                         const Point<T> &r) -> bool {
        return (r.x_ <= std::max(p.x_, q.x_) + cmp::precision<T>::epsilon &&
                r.x_ >= std::min(p.x_, q.x_) - cmp::precision<T>::epsilon &&
                r.y_ <= std::max(p.y_, q.y_) + cmp::precision<T>::epsilon &&
                r.y_ >= std::min(p.y_, q.y_) - cmp::precision<T>::epsilon);
    };

    if (std::abs(o1) <= cmp::precision<T>::epsilon && on_segment(a, b, c))
        return true;
    if (std::abs(o2) <= cmp::precision<T>::epsilon && on_segment(a, b, d))
        return true;
    if (std::abs(o3) <= cmp::precision<T>::epsilon && on_segment(c, d, a))
        return true;
    if (std::abs(o4) <= cmp::precision<T>::epsilon && on_segment(c, d, b))
        return true;

    return false;
}

} // namespace triangle 

#endif
