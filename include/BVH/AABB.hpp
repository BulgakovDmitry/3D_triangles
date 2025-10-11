#ifndef INCLUDE_AABB_HPP
#define INCLUDE_AABB_HPP

#include <algorithm>
#include <span>

#include "common/cmp.hpp"
#include "primitives/point.hpp"

namespace bounding_box {  
/* ---------- axis-aligned bounding box ---------- */
template <std::floating_point T> struct AABB {
    triangle::Point<T> p_min;
    triangle::Point<T> p_max;

    AABB()
        : p_min(triangle::Point(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(),
                      std::numeric_limits<T>::max())),
          p_max(triangle::Point(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(),
                      std::numeric_limits<T>::lowest())) {}

    explicit AABB(triangle::Point<T> pmin, triangle::Point<T> pmax) : p_min(pmin), p_max(pmax) {}

    static bool intersect(const AABB &a, const AABB &b) noexcept {
        return (a.p_min.get_x() <= b.p_max.get_x() && a.p_max.get_x() >= b.p_min.get_x()) &&
               (a.p_min.get_y() <= b.p_max.get_y() && a.p_max.get_y() >= b.p_min.get_y()) &&
               (a.p_min.get_z() <= b.p_max.get_z() && a.p_max.get_z() >= b.p_min.get_z());
    }

    void wrap_in_box_with(const AABB &point) {
        p_min = triangle::Point(std::min(p_min.get_x(), point.p_min.get_x()),
                      std::min(p_min.get_y(), point.p_min.get_y()),
                      std::min(p_min.get_z(), point.p_min.get_z()));

        p_max = triangle::Point(std::max(p_max.get_x(), point.p_max.get_x()),
                      std::max(p_max.get_y(), point.p_max.get_y()),
                      std::max(p_max.get_z(), point.p_max.get_z()));
    }

    triangle::Point<T> get_center() const noexcept {
        return triangle::Point((p_max.get_x() + p_min.get_x()) / 2, //  x
                     (p_max.get_y() + p_min.get_y()) / 2, //  y
                     (p_max.get_z() + p_min.get_z()) / 2  //  z
        );
    }
};

} // namespace bounding_box

#endif // INCLUDE_AABB_HPP
