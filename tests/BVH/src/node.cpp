#include <gtest/gtest.h>
#include <memory>
#include <span>
#include <vector>

#include "node.hpp"
#include "triangle.hpp"
#include "point.hpp"
#include "AABB.hpp"

using namespace triangle;
using NodeD = bin_tree::Node<double>;
using Tri   = triangle::Triangle<double>;
using AABBd = bounding_box::AABB<double>;
using P     = Point<double>;

static std::vector<Tri> make_triangles() {
    return {
        Tri(P{0,0,0}, P{1,0,0}, P{0,1,0}, /*id=*/1),
        Tri(P{2,0,0}, P{3,0,0}, P{2,1,0}, /*id=*/2)
    };
}

TEST(node, DefaultConstruction) {
    NodeD node;

    EXPECT_EQ(node.get_number_of_triangles(), 0u);
    EXPECT_EQ(node.get_triangles().size(), 0u);
    EXPECT_EQ(node.get_left().get(),  nullptr);
    EXPECT_EQ(node.get_right().get(), nullptr);
}

TEST(node, SetTrianglesMakesLeafLikeState) {
    NodeD node;
    auto triangles = make_triangles();
    node.set_triangles(std::span<Tri>(triangles));

    EXPECT_EQ(node.get_number_of_triangles(), 2u);
    EXPECT_EQ(node.get_triangles().size(), 2u);
    EXPECT_EQ(node.get_left().get(),  nullptr);
    EXPECT_EQ(node.get_right().get(), nullptr);
}

TEST(node, SetChildrenMakesInternalNodeLikeState) {
    NodeD node;

    auto left  = std::make_unique<NodeD>();
    auto right = std::make_unique<NodeD>();

    node.set_left(std::move(left));
    node.set_right(std::move(right));

    EXPECT_EQ(node.get_number_of_triangles(), 0u);
    EXPECT_NE(node.get_left().get(),  nullptr);
    EXPECT_NE(node.get_right().get(), nullptr);
}

TEST(node, AABBManagement) {
    NodeD node;
    AABBd bbox{P{0,0,0}, P{1,1,1}};

    node.set_box(bbox);

    const auto& result = node.get_box();
    EXPECT_DOUBLE_EQ(result.p_min.get_x(), 0.0);
    EXPECT_DOUBLE_EQ(result.p_min.get_y(), 0.0);
    EXPECT_DOUBLE_EQ(result.p_min.get_z(), 0.0);
    EXPECT_DOUBLE_EQ(result.p_max.get_x(), 1.0);
    EXPECT_DOUBLE_EQ(result.p_max.get_y(), 1.0);
    EXPECT_DOUBLE_EQ(result.p_max.get_z(), 1.0);
}

TEST(node, TriangleSpanIntegrity) {
    NodeD node;
    auto triangles = make_triangles();
    node.set_triangles(std::span<Tri>(triangles));

    auto span = node.get_triangles();
    ASSERT_EQ(span.size(), 2u);
    EXPECT_EQ(span[0].get_id(), 1u);
    EXPECT_EQ(span[1].get_id(), 2u);
}

TEST(node, StateTransitions_NoCrashOnSettingSingleChild) {
    NodeD node;
    node.set_left(std::make_unique<NodeD>()); 
    EXPECT_NE(node.get_left().get(), nullptr);
}