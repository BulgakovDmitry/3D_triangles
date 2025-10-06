#include "driver.hpp"
#include "BVH/BVH.hpp"
#include "primitives/triangle.hpp"
#include <cstdbool>
#include <iostream>
#include <vector>

using namespace triangle;

static std::vector<Triangle> get_input_data();
static void
print_numbers_of_intersecting_triangles(const std::set<std::size_t> &intersecting_triangles);

void triangle::driver() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<Triangle> triangles = get_input_data();

    bin_tree::BVH tree_root(std::move(triangles));
    tree_root.build();
    //tree_root.dump_graph();
    std::set<std::size_t> intersecting_triangles = tree_root.get_intersecting_triangles();

    print_numbers_of_intersecting_triangles(intersecting_triangles);
}

static std::vector<Triangle> get_input_data() {
    std::size_t N = 0;
    std::cin >> N;

    std::vector<Triangle> triangles;
    float x1, y1, z1, x2, y2, z2, x3, y3, z3;

    for (std::size_t i = 0; i < N; ++i) {
        std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

        triangles.push_back(Triangle(Point(x1, y1, z1), Point(x2, y2, z2), Point(x3, y3, z3), i));
    }
    return triangles;
}

static void
print_numbers_of_intersecting_triangles(const std::set<std::size_t> &intersecting_triangles) {
    for (std::size_t id : intersecting_triangles) {
        std::cout << id << "\n";
    }
}
