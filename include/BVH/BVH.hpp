#ifndef INCLUDE_BVH_HPP
#define INCLUDE_BVH_HPP

#include <memory>
#include <vector>
#include "node.hpp"
#include "triangle.hpp"
#include <set>

namespace bin_tree {

constexpr std::size_t max_number_of_triangles_in_leaf = 3;

enum class Axis {
    axis_x = 0,
    axis_y = 1,
    axis_z = 2
};

/* ---------- Bounding Volume Hierarchy ---------- */
class BVH {
private:
    std::unique_ptr<Node> root_ = nullptr;
    std::vector<Triangle> triangles_;
    std::set<std::size_t> intersecting_triangles_;

public:
    BVH(std::vector<Triangle>&& triangles) : triangles_(std::move(triangles)) {}
    void build();

private:
    std::unique_ptr<Node> build_node(size_t start, size_t end);
    Axis longest_axis(const AABB& box);
};

} // namespace bin_tree

#endif // INCLUDE_BVH_HPP