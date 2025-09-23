#include <gtest/gtest.h>

#include "geometry.hpp"

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
