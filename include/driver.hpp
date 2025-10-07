#ifndef INCLUDE_DRIVER_HPP
#define INCLUDE_DRIVER_HPP

#include <concepts>
#include <cstdbool>
#include <iostream>
#include <vector>

#include "BVH/BVH.hpp"
#include "primitives/triangle.hpp"

namespace triangle {

template <std::floating_point T>
static std::vector<Triangle<T>> get_input_data() {
    std::size_t N = 0;
    std::cin >> N;

    std::vector<Triangle<T>> triangles;
    float x1, y1, z1, x2, y2, z2, x3, y3, z3;

    for (std::size_t i = 0; i < N; ++i) {
        std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

        triangles.push_back(Triangle<T>(Point<T>(x1, y1, z1), Point<T>(x2, y2, z2), Point<T>(x3, y3, z3), i));
    }
    return triangles;
}

static void
print_numbers_of_intersecting_triangles(const std::set<std::size_t> &intersecting_triangles) {
    for (std::size_t id : intersecting_triangles) {
        std::cout << id << '\n';
    }
}

template <std::floating_point T>
void driver() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<Triangle<T>> triangles = get_input_data<T>();

    bin_tree::BVH tree_root(std::move(triangles));
    tree_root.build();
    // tree_root.dump_graph();
    std::set<std::size_t> intersecting_triangles = tree_root.get_intersecting_triangles();

    print_numbers_of_intersecting_triangles(intersecting_triangles);
}

} // namespace triangle

#endif
