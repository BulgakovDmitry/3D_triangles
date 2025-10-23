#ifndef INCLUDE_DRIVER_HPP
#define INCLUDE_DRIVER_HPP

#include <concepts>
#include <cstdbool>
#include <iostream>
#include <vector>

#include "BVH/BVH.hpp"
#include "primitives/triangle.hpp"

namespace triangle {

template <std::floating_point T> inline std::vector<Triangle<T>> get_input_data() {
    std::size_t N;
    if (!(std::cin >> N)) {
        throw std::runtime_error("Failed to read number of triangles.");
    }

    std::vector<Triangle<T>> triangles;
    float x1, y1, z1, x2, y2, z2, x3, y3, z3;

    for (std::size_t i = 0; i < N; ++i) {
        if (!(std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3)) {
            throw std::runtime_error("Failed to read triangle coordinates");
        }

        triangles.push_back(
            Triangle<T>(Point<T>(x1, y1, z1), Point<T>(x2, y2, z2), Point<T>(x3, y3, z3), i));
    }
    return triangles;
}

inline void
print_numbers_of_intersecting_triangles(const std::set<std::size_t> &intersecting_triangles) {
    for (std::size_t id : intersecting_triangles) {
        std::cout << id << '\n';
    }
}

template <std::floating_point T> std::set<std::size_t> driver(std::vector<Triangle<T>> triangles) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    bin_tree::BVH tree_root(std::move(triangles));
    tree_root.build();

    std::set<std::size_t> intersecting_triangles = tree_root.get_intersecting_triangles();

    return intersecting_triangles;
}

} // namespace triangle

#endif
