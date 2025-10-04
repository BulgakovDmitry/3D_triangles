#ifndef SEGMENT_TO_SEGMENT_HPP
#define SEGMENT_TO_SEGMENT_HPP

#include "common/cmp.hpp"
#include "primitives/point.hpp"
#include "primitives/vector.hpp"

inline bool check_segments_intersect_2d(const Point &a, const Point &b, const Point &c,
                                        const Point &d) {
    auto orient_2d_simple = [](const Point &p, const Point &q, const Point &r) -> double {
        return (q.get_x() - p.get_x()) * (r.get_y() - p.get_y()) -
               (q.get_y() - p.get_y()) * (r.get_x() - p.get_x());
    };

    double o1 = orient_2d_simple(a, b, c);
    double o2 = orient_2d_simple(a, b, d);
    double o3 = orient_2d_simple(c, d, a);
    double o4 = orient_2d_simple(c, d, b);

    // Common intersection (the segments intersect)
    if (((o1 > cmp::float_eps && o2 < -cmp::float_eps) ||
         (o1 < -cmp::float_eps && o2 > cmp::float_eps)) &&
        ((o3 > cmp::float_eps && o4 < -cmp::float_eps) ||
         (o3 < -cmp::float_eps && o4 > cmp::float_eps)))
        return true;

    // Collinear cases - the point lies on the segment
    auto on_segment = [](const Point &p, const Point &q, const Point &r) -> bool {
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
