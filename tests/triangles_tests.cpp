#include <gtest/gtest.h>

#include "triangle.hpp"

// --------------------------------------------------------------------------------------
//                           Tests intersect_3d 
// --------------------------------------------------------------------------------------
TEST(triangle, intersect_3d) {
    // arrange
    Triangle tr1(Point(0, 0, 2), Point(0, 1, 0), Point(1, 0, 0));
    Triangle tr2(Point(0, 0, 1), Point(0, 2, 0), Point(2, 0, 0));
    Triangle tr3(Point(0, 0, 3), Point(0, 2, 0), Point(2, 0, 0));

    // act, assert
    EXPECT_TRUE(tr1.intersect(tr2));
    EXPECT_FALSE(tr1.intersect(tr3));
}


// --------------------------------------------------------------------------------------
//                           Tests intersect_2d 
// --------------------------------------------------------------------------------------
static Point P(float x, float y, float z = 0.0) { return Point{x, y, z}; }

TEST(Intersect2D, Disjoint_NoOverlap) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(3,3), P(4,3), P(3,4) };

    // act, assert
    EXPECT_FALSE(A.intersect_2d(B));
    EXPECT_FALSE(B.intersect_2d(A));
}

TEST(Intersect2D, Containment_B_inside_A) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(0.2,0.2), P(0.6,0.2), P(0.2,0.6) }; 

    // act, assert
    EXPECT_TRUE(A.intersect_2d(B));
    EXPECT_TRUE(B.intersect_2d(A));
}

TEST(Intersect2D, Containment_OrientationDoesNotMatter) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(0.2,0.6), P(0.6,0.2), P(0.2,0.2) };

    // act, assert
    EXPECT_TRUE(A.intersect_2d(B));
    EXPECT_TRUE(B.intersect_2d(A));
}

TEST(Intersect2D, EdgeEdge_ProperCross_NoVertexInside) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(1,2) };
    Triangle B{ P(-0.5,0.5), P(2.5,0.5), P(1.5,-1) };

    // act, assert
    EXPECT_TRUE(A.intersect_2d(B));
    EXPECT_TRUE(B.intersect_2d(A));
}

TEST(Intersect2D, Touch_VertexOnEdge_PointContact) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(1,0), P(1.1,-0.2), P(0.9,-0.2) };

    // act, assert
    EXPECT_TRUE(A.intersect_2d(B));
    EXPECT_TRUE(B.intersect_2d(A));
}

TEST(Intersect2D, Collinear_OverlappingEdges) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(0,0), P(2,0), P(3,1) }; 

    // act, assert
    EXPECT_TRUE(A.intersect_2d(B));
    EXPECT_TRUE(B.intersect_2d(A));
}

TEST(Intersect2D, Touch_AtSingleSharedVertexOnly) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(2,0), P(3,0), P(2,1) };

    // act, assert
    EXPECT_TRUE(A.intersect_2d(B));
    EXPECT_TRUE(B.intersect_2d(A));
}

TEST(Intersect2D, Disjoint_CloseButSeparated) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(2.1,0.0), P(3.0,0.0), P(2.1,0.9) };

    // act, assert
    EXPECT_FALSE(A.intersect_2d(B));
    EXPECT_FALSE(B.intersect_2d(A));
}