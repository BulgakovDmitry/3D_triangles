#include <gtest/gtest.h>
#include "point.hpp"

using namespace triangle;

// --------------------------------------------------------------------------------------
//                           Tests class Point
// --------------------------------------------------------------------------------------

TEST(Point, EqualityOperator) {
    Point<float> a(1.f, 1.f, 1.f);
    Point<float> b(2.f, 1.f, 1.f);
    Point<float> c(1.f, 1.f, 1.f);

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a == c);
}