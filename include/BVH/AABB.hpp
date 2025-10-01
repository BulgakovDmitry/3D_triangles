#ifndef INCLUDE_AABB_HPP
#define INCLUDE_AABB_HPP

#include "primitives/point.hpp"
#include "../../common/cmp.hpp"
#include <algorithm>


namespace bin_tree {

/* ---------- axis-aligned bounding box ---------- */
struct AABB {
    Point p_min;
    Point p_max;

    AABB() : p_min(Point(float_constants::float_min, float_constants::float_min, float_constants::float_min)),
             p_max(Point(float_constants::float_max, float_constants::float_max, float_constants::float_max)) {}
            
    explicit AABB(Point pmin, Point pmax) : p_min(pmin), p_max(pmax) {}
    
    bool intersect(const AABB& a, const AABB& b) const noexcept {
        return (a.p_min.get_x() <= b.p_max.get_x() && a.p_max.get_x() >= b.p_min.get_x())
            && (a.p_min.get_y() <= b.p_max.get_y() && a.p_max.get_y() >= b.p_min.get_y())
            && (a.p_min.get_z() <= b.p_max.get_z() && a.p_max.get_z() >= b.p_min.get_z());
    }
    
    void wrap_in_box_with(const AABB& point) {
        p_min = Point(
            std::min(p_min.get_x(), point.p_min.get_x()),
            std::min(p_min.get_y(), point.p_min.get_y()),
            std::min(p_min.get_z(), point.p_min.get_z())
        );

        p_max = Point(
            std::min(p_max.get_x(), point.p_max.get_x()),
            std::min(p_max.get_y(), point.p_max.get_y()),
            std::min(p_max.get_z(), point.p_max.get_z())
        );
    }

    Point get_center() const noexcept {
        return Point(
            (p_max.get_x() + p_min.get_x()) / 2, //  x
            (p_max.get_y() + p_min.get_y()) / 2, //  y
            (p_max.get_z() + p_min.get_z()) / 2  //  z
        );
    }
};

} // namespace bin_tree

#endif // INCLUDE_AABB_HPP