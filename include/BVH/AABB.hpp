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
        return (a.p_min.x_ <= b.p_max.x_ && a.p_max.x_ >= b.p_min.x_) &&
               (a.p_min.y_ <= b.p_max.y_ && a.p_max.y_ >= b.p_min.y_) &&
               (a.p_min.z_ <= b.p_max.z_ && a.p_max.z_ >= b.p_min.z_);
    }

    void wrap_in_box_with(const AABB &point) {
        p_min = triangle::Point(std::min(p_min.x_, point.p_min.x_),
                                std::min(p_min.y_, point.p_min.y_),
                                std::min(p_min.z_, point.p_min.z_));

        p_max = triangle::Point(std::max(p_max.x_, point.p_max.x_),
                                std::max(p_max.y_, point.p_max.y_),
                                std::max(p_max.z_, point.p_max.z_));
    }

    triangle::Point<T> get_center() const noexcept {
        return triangle::Point((p_max.x_ + p_min.x_) / 2, //  x
                               (p_max.y_ + p_min.y_) / 2, //  y
                               (p_max.z_ + p_min.z_) / 2  //  z
        );
    }
};

} // namespace bounding_box

#endif // INCLUDE_AABB_HPP
