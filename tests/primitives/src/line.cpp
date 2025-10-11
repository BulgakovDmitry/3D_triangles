#include <gtest/gtest.h>
#include "line.hpp"

using namespace triangle;

// --------------------------------------------------------------------------------------
//                           Tests class Line
// --------------------------------------------------------------------------------------

TEST(Line, orthogonal) {
    Line<double> X(Vector<double>(0,0,0), Vector<double>(1,0,0));
    Line<double> Y(Vector<double>(0,0,0), Vector<double>(0,1,0));
    EXPECT_TRUE(X.orthogonal(Y));

    Line<double> D(Vector<double>(0,0,0), Vector<double>(1,1,0));
    EXPECT_FALSE(X.orthogonal(D));
}

TEST(Line, collinear) {
    Line<double> L1(Vector<double>(0,0,0), Vector<double>(1,0,0));
    Line<double> L2(Vector<double>(0,1,0), Vector<double>(2,0,0));
    EXPECT_TRUE(L1.collinear(L2));

    Line<double> L3(Vector<double>(0,0,1), Vector<double>(1,1,0));
    EXPECT_FALSE(L1.collinear(L3));
}

TEST(Line, contains_point) {
    Line<double> L(Vector<double>(0,0,0), Vector<double>(1,2,3));

    Point<double> on(2,4,6);
    EXPECT_TRUE(L.contains(on));

    Point<double> off(1,0,0);
    EXPECT_FALSE(L.contains(off));
}

TEST(Line, contains_line) {
    Line<double> L(Vector<double>(1,1,1), Vector<double>(1,2,0));

    Vector<double> OP_on(3,5,1);
    EXPECT_TRUE(L.contains(OP_on));

    Vector<double> OP_off(3,5,2);
    EXPECT_FALSE(L.contains(OP_off));
}

TEST(Line, EqualityOperator) {
    Line<double> L1(Vector<double>(0,0,0), Vector<double>(1,1,0));
    Line<double> L2(Vector<double>(2,2,0), Vector<double>(2,2,0));
    EXPECT_TRUE(L1 == L2);

    Line<double> L3(Vector<double>(0,1,0), Vector<double>(1,1,0));
    EXPECT_FALSE(L1 == L3);

    Line<double> L4(Vector<double>(0,0,1), Vector<double>(0,0,1));
    EXPECT_FALSE(L1 == L4);
}