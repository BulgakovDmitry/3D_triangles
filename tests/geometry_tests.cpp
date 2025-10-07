#include <gtest/gtest.h>

#include "primitives/point.hpp"
#include "primitives/vector.hpp"
#include "primitives/line.hpp"

// --------------------------------------------------------------------------------------
//                           Tests class Point
// --------------------------------------------------------------------------------------

TEST(points, equal_points) {
    // arrange
    Point<float> a(1.f, 1.f, 1.f);
    Point<float> b(2.f, 1.f, 1.f);
    Point<float> c(1.f, 1.f, 1.f);

    // act, assert
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a == c);
}

// --------------------------------------------------------------------------------------
//                           Tests class Vector
// --------------------------------------------------------------------------------------

TEST(vectors, orthogonal_vectors) {
    // arrange
    Point<float>  a(1.f, 1.f, 1.f);
    Point<float>  b(2.f, 1.f, 1.f);
    Point<float>  c(3.f, 2.f, 1.f);
    Point<float>  d(3.f, 6.f, 1.f);
    Vector<float> v1(a, b);
    Vector<float> v2(c, d);

    // act, assert
    ASSERT_TRUE(v1.orthogonal(v2));
}

TEST(vectors, check_abs) {
    // arrange
    Vector<float> v(3, 4, 0);

    // act, assert
    EXPECT_TRUE(v.abs() == 5);
}

TEST(vectors, collinear_vectors) {
    // arrange
    Vector<float> v1(1, 2, 3);
    Vector<float> v2(3, 6, 9);

    // act, assert
    ASSERT_TRUE(v1.collinear(v2));
}

TEST(vectors, nul_vectors) {
    // arrange
    Point<float>  a(1, 1, 1);
    Point<float>  b(2, 1, 1);
    Vector<float> v1(a, b);
    Vector<float> v2(0, 0, 0);

    // act, assert
    EXPECT_FALSE(v1.is_nul());
    EXPECT_TRUE(v2.is_nul());
}

TEST(vectors, projection) {
    // arrange
    Vector<float> v1(1, 1, 0);
    Vector<float> u1(2, 0, 0);

    // act
    Vector<float> proj1 = v1.projection(u1);

    // assert
    EXPECT_NEAR(proj1.get_x(), 1.0, 1e-6);
    EXPECT_NEAR(proj1.get_y(), 0.0, 1e-6);
    EXPECT_NEAR(proj1.get_z(), 0.0, 1e-6);

    // arrange
    Vector<float> v2(0, 5, 1);
    Vector<float> u2(0, 1, 0);

    // act
    Vector<float> proj2 = v2.projection(u2);

    // assert
    EXPECT_NEAR(proj2.get_x(), 0.0, 1e-6);
    EXPECT_NEAR(proj2.get_y(), 5.0, 1e-6);
    EXPECT_NEAR(proj2.get_z(), 0.0, 1e-6);

    // arrange
    Vector<float> v3(3, 4, 0);
    Vector<float> u3(0, 0, 1);

    // act
    Vector<float> proj3 = v3.projection(u3);

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
    Point<float>  a(1, 1, 1);
    Point<float>  b(2, 1, 1);
    Point<float>  c(3, 2, 1);
    Point<float>  d(3, 6, 1);
    Vector<float> l1(a, b);
    Vector<float> l2(c, d);

    // act, assert
    ASSERT_TRUE(l1.orthogonal(l2));
}

TEST(lines, collinear_lines) {
    // arrange
    Vector<float> v1(1, 2, 3);
    Vector<float> v2(3, 6, 9);
    Point<float>  a(1, 1, 1);
    Point<float>  b(2, 1, 1);
    Line<float>   l1(a, v1);
    Line<float>   l2(b, v2);

    // act, assert
    ASSERT_TRUE(v1.collinear(v2));
}

TEST(lines, line_contain_point) {
    // arrange
    Point<float>  a(0, 0, 0);
    Point<float>  b(2, 1, 1);
    Point<float>  c(3, 6, 9);
    Vector<float> v1(1, 2, 3);
    Line<float>   l1(a, v1);

    // act, assert
    EXPECT_TRUE(l1.contains(c));
    EXPECT_FALSE(l1.contains(b));
}

TEST(lines, equal_lines) {
    // arrange
    Point<float>  a(0, 0, 0);
    Point<float>  b(-1, -2, -3);
    Vector<float> v1(1, 2, 3);
    Vector<float> v2(2, 4, 6);
    Line<float>   l1(a, v1);
    Line<float>   l2(b, v2);

    // act, assert
    ASSERT_TRUE(l2 == l1);
}
