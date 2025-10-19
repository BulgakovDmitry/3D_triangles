#include <gtest/gtest.h>

#include "triangle.hpp"
#include "triangle_to_triangle.hpp"
#include "triangle_to_triangle_2d.hpp"
#include "point.hpp"

using namespace triangle;

// --------------------------------------------------------------------------------------
//                           Tests intersect_2d
// --------------------------------------------------------------------------------------

static Point<float> P(float x, float y, float z = 0.0) { return Point<float>{x, y, z}; }

TEST(intersect_2d, Disjoint_NoOverlap) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(3,3), P(4,3), P(3,4) };

    // act, assert
    EXPECT_FALSE(intersect_2d(A, B));
    EXPECT_FALSE(intersect_2d(B, A));
}

TEST(intersect_2d, Containment_B_inside_A) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(0.2,0.2), P(0.6,0.2), P(0.2,0.6) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(intersect_2d, Containment_OrientationDoesNotMatter) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(0.2,0.6), P(0.6,0.2), P(0.2,0.2) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(intersect_2d, EdgeEdge_ProperCross_NoVertexInside) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(1,2) };
    Triangle<float> B{ P(-0.5,0.5), P(2.5,0.5), P(1.5,-1) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(intersect_2d, Touch_VertexOnEdge_PointContact) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(1,0), P(1.1,-0.2), P(0.9,-0.2) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(intersect_2d, Collinear_OverlappingEdges) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(0,0), P(2,0), P(3,1) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(intersect_2d, Touch_AtSingleSharedVertexOnly) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(2,0), P(3,0), P(2,1) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(intersect_2d, Disjoint_CloseButSeparated) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(2.1,0.0), P(3.0,0.0), P(2.1,0.9) };

    // act, assert
    EXPECT_FALSE(intersect_2d(A, B));
    EXPECT_FALSE(intersect_2d(B, A));
}
