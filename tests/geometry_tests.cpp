#include <gtest/gtest.h>

#include "primitives/point.hpp"
#include "primitives/vector.hpp"
#include "primitives/line.hpp"

// --------------------------------------------------------------------------------------
//                           Tests class Point
// --------------------------------------------------------------------------------------

TEST(points, equal_points) {
    // arrange
    Point a(1, 1, 1);
    Point b(2, 1, 1);
    Point c(1, 1, 1);

    // act, assert
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a == c);
}

// --------------------------------------------------------------------------------------
//                           Tests class Vector
// --------------------------------------------------------------------------------------

TEST(vectors, orthogonal_vectors) {
    // arrange
    Point  a(1, 1, 1);
    Point  b(2, 1, 1);

    Point  c(3, 2, 1);
    Point  d(3, 6, 1);

    Vector v1(a, b);
    Vector v2(c, d);


    // act, assert
    ASSERT_TRUE(v1.orthogonal(v2));
}

TEST(vectors, check_abs) {
    // arrange
    Vector v(3, 4, 0);

    // act, assert
    EXPECT_TRUE(v.abs() == 5);
}

TEST(vectors, collinear_vectors) {
    // arrange
    Vector v1(1, 2, 3);
    Vector v2(3, 6, 9);

    // act, assert
    ASSERT_TRUE(v1.collinear(v2));
}

TEST(vectors, valid_vectors) {
    // arrange
    Point  a(1, 1, 1);
    Point  b(2, 1, 1);

    Vector v1(a, b);

    // assert
    EXPECT_TRUE(v1.valid());
}

TEST(vectors, nul_vectors) {
    // arrange
    Point  a(1, 1, 1);
    Point  b(2, 1, 1);

    Vector v1(a, b);
    Vector v2(0, 0, 0);

    // act, assert
    EXPECT_FALSE(v1.is_nul());
    EXPECT_TRUE(v2.is_nul());
}

TEST(vectors, projection) {
    // arrange
    Vector v1(1, 1, 0);
    Vector u1(2, 0, 0);

    // act
    Vector proj1 = v1.projection(u1);

    // assert
    EXPECT_NEAR(proj1.get_x(), 1.0, 1e-6);
    EXPECT_NEAR(proj1.get_y(), 0.0, 1e-6);
    EXPECT_NEAR(proj1.get_z(), 0.0, 1e-6);

    // arrange
    Vector v2(0, 5, 1);
    Vector u2(0, 1, 0);

    // act
    Vector proj2 = v2.projection(u2);

    // assert
    EXPECT_NEAR(proj2.get_x(), 0.0, 1e-6);
    EXPECT_NEAR(proj2.get_y(), 5.0, 1e-6);
    EXPECT_NEAR(proj2.get_z(), 0.0, 1e-6);

    // arrange
    Vector v3(3, 4, 0);
    Vector u3(0, 0, 1);

    // act
    Vector proj3 = v3.projection(u3);

    // assert
    EXPECT_NEAR(proj3.get_x(), 0.0, 1e-6);
    EXPECT_NEAR(proj3.get_y(), 0.0, 1e-6);
    EXPECT_NEAR(proj3.get_z(), 0.0, 1e-6);
}

// --------------------------------------------------------------------------------------
//                           Tests class Line
// --------------------------------------------------------------------------------------

TEST(lines, orthogonal_lines) {
    // arrange
    Point  a(1, 1, 1);
    Point  b(2, 1, 1);

    Point  c(3, 2, 1);
    Point  d(3, 6, 1);

    Vector l1(a, b);
    Vector l2(c, d);

    // act, assert
    ASSERT_TRUE(l1.orthogonal(l2));
}

TEST(lines, collinear_lines) {
    // arrange
    Vector v1(1, 2, 3);
    Vector v2(3, 6, 9);

    Point  a(1, 1, 1);
    Point  b(2, 1, 1);

    Line   l1(a, v1);
    Line   l2(b, v2);

    // act, assert
    ASSERT_TRUE(v1.collinear(v2));
}

TEST(lines, valid_lines) {
    // arrange
    Point  a(1, 1, 1);
    Point  b(2, 1, 1);

    Vector v1(a, b);

    Line   l1(a, v1);

    // assert
    EXPECT_TRUE(l1.valid());
}

TEST(lines, line_contain_point) {
    // arrange
    Point  a(0, 0, 0);
    Point  b(2, 1, 1);
    Point  c(3, 6, 9);

    Vector v1(1, 2, 3);

    Line   l1(a, v1);

    // act, assert
    EXPECT_TRUE(l1.contains(c));
    EXPECT_FALSE(l1.contains(b));
}

TEST(lines, equal_lines) {
    // arrange
    Point  a(0, 0, 0);
    Point  b(-1, -2, -3);

    Vector v1(1, 2, 3);
    Vector v2(2, 4, 6);

    Line   l1(a, v1);
    Line   l2(b, v2);

    // act, assert
    ASSERT_TRUE(l2 == l1);
}
