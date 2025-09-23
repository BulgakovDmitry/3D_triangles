#include <gtest/gtest.h>

#include "geometry.hpp"

TEST(points, equal_points) {
    // arrange
    Point a(1, 1, 1);
    Point b(2, 1, 1);
    Point c(1, 1, 1);

    // act, assert
    EXPECT_FALSE(a.equal(b));
    EXPECT_TRUE(a.equal(c));
}

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

    Point  c(3, 2, 1);
    Point  d(3, 6, 1);

    Vector v1(a, b);
    Vector v2(c, d);

    // act
    v2.erase();

    // assert
    EXPECT_TRUE(v1.valid());
    EXPECT_FALSE(v2.valid());
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

    Point  c(3, 2, 1);
    Point  d(3, 6, 1);

    Vector v1(a, b);
    Vector v2(c, d);

    Line   l1(a, v1);
    Line   l2(b, v2);

    // act
    l2.erase();

    // assert
    EXPECT_TRUE(l1.valid());
    EXPECT_FALSE(l2.valid());
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
    ASSERT_TRUE(l2.equal(l1));
}
