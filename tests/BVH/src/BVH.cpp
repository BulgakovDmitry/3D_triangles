#include <gtest/gtest.h>
#include <vector>

#include "BVH.hpp"
#include "triangle.hpp"
#include "point.hpp"

using BVHD = bin_tree::BVH<double>;
using Tri  = triangle::Triangle<double>;
using P    = Point<double>;

static std::vector<Tri> make_grid_triangles() {
    return {
        Tri(P{0,0,0}, P{1,0,0}, P{0,1,0}, /*id=*/1),
        Tri(P{2,0,0}, P{3,0,0}, P{2,1,0}, /*id=*/2),
        Tri(P{4,0,0}, P{5,0,0}, P{4,1,0}, /*id=*/3),
        Tri(P{0,2,0}, P{1,2,0}, P{0,3,0}, /*id=*/4),
        Tri(P{2,2,0}, P{3,2,0}, P{2,3,0}, /*id=*/5),
        Tri(P{4,2,0}, P{5,2,0}, P{4,3,0}, /*id=*/6)
    };
}

TEST(BVH, ConstructionAndBuild) {
    auto triangles = make_grid_triangles();
    BVHD bvh(std::move(triangles));

    EXPECT_TRUE(bvh.get_intersecting_triangles().empty());

    EXPECT_NO_THROW(bvh.build());
    EXPECT_NO_THROW({
        auto& s = bvh.get_intersecting_triangles();
        (void)s;
    });
}

TEST(BVH, EmptyTriangleList) {
    std::vector<Tri> empty_triangles;
    BVHD bvh(std::move(empty_triangles));

    EXPECT_NO_THROW(bvh.build());
    EXPECT_TRUE(bvh.get_intersecting_triangles().empty());
}

TEST(BVH, SingleTriangle) {
    std::vector<Tri> single_triangle = {
        Tri(P{0,0,0}, P{1,0,0}, P{0,1,0}, /*id=*/1)
    };

    BVHD bvh(std::move(single_triangle));
    bvh.build();

    const auto& isects = bvh.get_intersecting_triangles();
    EXPECT_TRUE(isects.empty());
}


TEST(BVH, NoIntersectionsForSeparatedTriangles) {
    std::vector<Tri> separated_triangles = {
        Tri(P{0,0,0},  P{1,0,0},  P{0,1,0},  /*id=*/1),
        Tri(P{10,0,0}, P{11,0,0}, P{10,1,0}, /*id=*/2)
    };

    BVHD bvh(std::move(separated_triangles));
    bvh.build();

    const auto& isects = bvh.get_intersecting_triangles();
    EXPECT_TRUE(isects.empty());
}

TEST(BVH, IntersectingTrianglesDetection) {
    std::vector<Tri> intersecting_triangles = {
        Tri(P{0,0,0}, P{2,0,0}, P{0,2,0}, /*id=*/1),
        Tri(P{1,1,0}, P{3,1,0}, P{1,3,0}, /*id=*/2)
    };

    BVHD bvh(std::move(intersecting_triangles));
    bvh.build();

    const auto& isects = bvh.get_intersecting_triangles();
    EXPECT_FALSE(isects.empty());
    EXPECT_EQ(isects.size(), 2u);
    EXPECT_NE(isects.find(1u), isects.end());
    EXPECT_NE(isects.find(2u), isects.end());
}

TEST(BVH, SelfIntersectionNotReported) {
    std::vector<Tri> single_triangle = {
        Tri(P{0,0,0}, P{1,0,0}, P{0,1,0}, /*id=*/1)
    };

    BVHD bvh(std::move(single_triangle));
    bvh.build();

    const auto& isects = bvh.get_intersecting_triangles();
    EXPECT_TRUE(isects.empty());
}


TEST(BVH, LargeNumberOfTriangles) {
    std::vector<Tri> large_set;
    const int num_triangles = 1000;

    large_set.reserve(num_triangles);
    for (int i = 0; i < num_triangles; ++i) {
        double x = static_cast<double>(i);
        large_set.emplace_back(P{x,0,0}, P{x+1,0,0}, P{x,1,0}, /*id=*/static_cast<std::size_t>(i));
    }

    BVHD bvh(std::move(large_set));
    EXPECT_NO_THROW(bvh.build());

    EXPECT_NO_THROW({
        auto& s = bvh.get_intersecting_triangles();
        (void)s;
    });
}