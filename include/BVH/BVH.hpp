#ifndef INCLUDE_BVH_HPP
#define INCLUDE_BVH_HPP

#include "node.hpp"
#include "triangle.hpp"
#include <memory>
#include <set>
#include <vector>

namespace BVH {

constexpr std::size_t max_number_of_triangles_in_leaf = 3;

enum class Axis { axis_x = 0, axis_y = 1, axis_z = 2 };

/* ---------- Bounding Volume Hierarchy ---------- */
class BVH {
private:
    std::unique_ptr<Node> root_ = nullptr;
    std::vector<Triangle> triangles_;
    std::set<std::size_t> intersecting_triangles_;

public:
    BVH(std::vector<Triangle> &&triangles) : triangles_(std::move(triangles)) {}

private:
};

inline Axis longest_axis(const AABB &box) {
    Vector v(box.p_max, box.p_min);
    float  v_x = v.get_x();
    float  v_y = v.get_y();
    float  v_z = v.get_z();

    if (v_x >= v_y && v_x >= v_z) {
        return Axis::axis_x;
    } else if (v_y >= v_z) {
        return Axis::axis_y;
    } else {
        return Axis::axis_z;
    }
}

} // namespace BVH

#endif // INCLUDE_BVH_HPP