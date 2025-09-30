#include "driver.hpp"
#include "colors.hpp"
#include "triangle.hpp"
#include <cstdbool>
#include <iostream>
#include <vector>
#include "BVH/BVH.hpp"

static std::vector<Triangle> get_input_data();
// static void dump (const std::vector<Triangle>& vec);
static std::vector<std::size_t>
            get_numbers_of_intersecting_triangles(const std::vector<Triangle> &triangles);
static void print_numbers_of_intersecting_triangles(const std::vector<std::size_t> &vec);

void driver() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<Triangle>    triangles = get_input_data();

    BVH::BVH tree_root(std::move(triangles));
    //tree_root.build()

    //std::vector<std::size_t> num       = get_numbers_of_intersecting_triangles(triangles);

    // dump(triangles);

    //print_numbers_of_intersecting_triangles(num);
}

static std::vector<Triangle> get_input_data() {
    std::size_t N = 0;
    std::cin >> N;

    std::vector<Triangle> triangles;
    float                 x1, y1, z1, x2, y2, z2, x3, y3, z3;

    for (std::size_t i = 0; i < N; ++i) {
        std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

        triangles.push_back(Triangle(Point(x1, y1, z1), Point(x2, y2, z2), Point(x3, y3, z3)));
    }
    return triangles;
}

// static void dump(const std::vector<Triangle>& vec) {
//     std::size_t N = vec.size();
//     for (std::size_t i = 0; i < N; ++i) {
//         std::cout << RED << i + 1<< " " << RESET;
//         vec[i].print();
//     }
// }

static std::vector<std::size_t>
get_numbers_of_intersecting_triangles(const std::vector<Triangle> &triangles) {

    std::size_t       N = triangles.size();
    std::vector<bool> hit(N, false);

    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i + 1; j < N; ++j) {
            if (triangles[i].intersect(triangles[j])) {
                hit[i] = true;
                hit[j] = true;
            }
        }
    }

    std::vector<std::size_t> numbers_of_intersecting_triangles;
    numbers_of_intersecting_triangles.reserve(N);
    for (std::size_t i = 0; i < N; ++i) {
        if (hit[i])
            numbers_of_intersecting_triangles.push_back(i + 1);
    }
    return numbers_of_intersecting_triangles;
}

static void print_numbers_of_intersecting_triangles(const std::vector<std::size_t> &vec) {
    std::size_t N = vec.size();
    for (std::size_t i = 0; i < N; ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}
