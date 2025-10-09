#include <gtest/gtest.h>
#include <algorithm>
#include <cmath>
#include <utility>

#include "triangle.hpp"

// --------------------------------------------------------------------------------------
//                           Tests class Triangle
// --------------------------------------------------------------------------------------

using triangle::Triangle;
using triangle::TypeTriangle;
using T = double;

static constexpr T kEps = static_cast<T>(1e-9);

TEST(Triangle, are_collinear_1) {
    Point<T> a(0,0,0);
    Point<T> b(1,1,1);
    Point<T> c(2,2,2);
    EXPECT_TRUE(triangle::are_collinear(a,b,c));
}

TEST(Triangle, are_collinear_2) {
    Point<T> a(0,0,0);
    Point<T> b(1,0,0);
    Point<T> c(0,1,0);
    EXPECT_FALSE(triangle::are_collinear(a,b,c));
}

TEST(Triangle, get_interval_1) {
    Point<T> p0(0,0,0);
    Point<T> p1(1,0,0);
    Point<T> p2(3,0,0);
    Triangle<T> tri(p0,p1,p2);

    EXPECT_EQ(tri.get_type(), TypeTriangle::interval);

    auto seg = tri.get_interval();
    EXPECT_TRUE( (seg.first==0 && seg.second==2) || (seg.first==2 && seg.second==0) );
}

TEST(Triangle, get_interval_2) {
    Triangle<T> tri(Point<T>(0,0,0), Point<T>(1,0,0), Point<T>(0,1,0));
    EXPECT_EQ(tri.get_type(), TypeTriangle::triangle);
    auto seg = tri.get_interval();
    EXPECT_EQ(seg.first, 0u);
    EXPECT_EQ(seg.second, 0u);
}

TEST(Triangle, get_vertices) {
    Point<T> p0(0,0,0);
    Point<T> p1(1,0,0);
    Point<T> p2(0,1,0);
    Triangle<T> tri(p0,p1,p2);

    tri.swap_vertices(0,2);
    const auto& v = tri.get_vertices();
    EXPECT_NEAR(v[0].get_x(), p2.get_x(), kEps);
    EXPECT_NEAR(v[0].get_y(), p2.get_y(), kEps);
    EXPECT_NEAR(v[0].get_z(), p2.get_z(), kEps);

    EXPECT_NEAR(v[1].get_x(), p1.get_x(), kEps);
    EXPECT_NEAR(v[1].get_y(), p1.get_y(), kEps);
    EXPECT_NEAR(v[1].get_z(), p1.get_z(), kEps);

    EXPECT_NEAR(v[2].get_x(), p0.get_x(), kEps);
    EXPECT_NEAR(v[2].get_y(), p0.get_y(), kEps);
    EXPECT_NEAR(v[2].get_z(), p0.get_z(), kEps);
}

TEST(Triangle, rotate_clockwise_1) {
    Point<T> p0(0,0,0);
    Point<T> p1(1,0,0);
    Point<T> p2(0,1,0);
    Triangle<T> tri(p0,p1,p2);

    tri.rotate_clockwise();
    const auto& v = tri.get_vertices();

    bool leftCycle =
        std::fabs(v[0].get_x()-p1.get_x())<=kEps && std::fabs(v[0].get_y()-p1.get_y())<=kEps && std::fabs(v[0].get_z()-p1.get_z())<=kEps &&
        std::fabs(v[1].get_x()-p2.get_x())<=kEps && std::fabs(v[1].get_y()-p2.get_y())<=kEps && std::fabs(v[1].get_z()-p2.get_z())<=kEps &&
        std::fabs(v[2].get_x()-p0.get_x())<=kEps && std::fabs(v[2].get_y()-p0.get_y())<=kEps && std::fabs(v[2].get_z()-p0.get_z())<=kEps;

    bool rightCycle =
        std::fabs(v[0].get_x()-p2.get_x())<=kEps && std::fabs(v[0].get_y()-p2.get_y())<=kEps && std::fabs(v[0].get_z()-p2.get_z())<=kEps &&
        std::fabs(v[1].get_x()-p0.get_x())<=kEps && std::fabs(v[1].get_y()-p0.get_y())<=kEps && std::fabs(v[1].get_z()-p0.get_z())<=kEps &&
        std::fabs(v[2].get_x()-p1.get_x())<=kEps && std::fabs(v[2].get_y()-p1.get_y())<=kEps && std::fabs(v[2].get_z()-p1.get_z())<=kEps;

    EXPECT_TRUE(leftCycle || rightCycle);
}

TEST(Triangle, rotate_clockwise_2) {
    Point<T> p0(0,0,0);
    Point<T> p1(1,0,0);
    Point<T> p2(0,1,0);
    Triangle<T> tri(p0,p1,p2);

    tri.rotate_clockwise();
    tri.rotate_clockwise();
    tri.rotate_clockwise();

    const auto& v = tri.get_vertices();
    EXPECT_NEAR(v[0].get_x(), p0.get_x(), kEps);
    EXPECT_NEAR(v[0].get_y(), p0.get_y(), kEps);
    EXPECT_NEAR(v[0].get_z(), p0.get_z(), kEps);

    EXPECT_NEAR(v[1].get_x(), p1.get_x(), kEps);
    EXPECT_NEAR(v[1].get_y(), p1.get_y(), kEps);
    EXPECT_NEAR(v[1].get_z(), p1.get_z(), kEps);

    EXPECT_NEAR(v[2].get_x(), p2.get_x(), kEps);
    EXPECT_NEAR(v[2].get_y(), p2.get_y(), kEps);
    EXPECT_NEAR(v[2].get_z(), p2.get_z(), kEps);
}

TEST(Triangle, get_box) {
    Point<T> p0( 2, -1,  3);
    Point<T> p1(-4,  5, -2);
    Point<T> p2( 0,  2,  1);
    Triangle<T> tri(p0,p1,p2);

    auto box = tri.get_box();

    T minx = std::min({p0.get_x(), p1.get_x(), p2.get_x()});
    T miny = std::min({p0.get_y(), p1.get_y(), p2.get_y()});
    T minz = std::min({p0.get_z(), p1.get_z(), p2.get_z()});

    T maxx = std::max({p0.get_x(), p1.get_x(), p2.get_x()});
    T maxy = std::max({p0.get_y(), p1.get_y(), p2.get_y()});
    T maxz = std::max({p0.get_z(), p1.get_z(), p2.get_z()});

    EXPECT_NEAR(box.p_min.get_x(), minx, kEps);
    EXPECT_NEAR(box.p_min.get_y(), miny, kEps);
    EXPECT_NEAR(box.p_min.get_z(), minz, kEps);

    EXPECT_NEAR(box.p_max.get_x(), maxx, kEps);
    EXPECT_NEAR(box.p_max.get_y(), maxy, kEps);
    EXPECT_NEAR(box.p_max.get_z(), maxz, kEps);
}
