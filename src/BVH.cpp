#include "BVH/BVH.hpp"
#include "BVH/node.hpp"
#include "triangle.hpp"

using namespace bin_tree;

Axis BVH::longest_axis(const AABB &box) {
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

std::unique_ptr<Node> BVH::build_node(std::size_t start, std::size_t end) {
    std::span<Triangle> triangles(triangles_.begin() + start, triangles_.begin() + end);

    auto                node = std::make_unique<Node>();

    AABB                box  = calculate_bounding_box(triangles);
    node->set_box(box);

    if (end - start <= max_number_of_triangles_in_leaf) {
        node->set_triangles(triangles);
        return node;
    }

    // Axis axis = longest_axis(box);

    // // std::sort(
    //     triangles.begin(),
    //     triangles.end(),
    //     [axis](const Triangle& a, const Triangle& b) {
    //         return a.get_box().get_center()[static_cast<std::size_t>(axis)] <
    //         b.get_box().get_center()[static_cast<std::size_t>(axis)];
    //     }
    // );

    // TODO
}
