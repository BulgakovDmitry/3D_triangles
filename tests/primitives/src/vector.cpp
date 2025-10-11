#include <gtest/gtest.h>
#include "vector.hpp"

using namespace triangle;

// --------------------------------------------------------------------------------------
//                           Tests class Vector
// --------------------------------------------------------------------------------------

TEST(Vector, scalar_multiply) {
    Vector<double> v(1.5, -2.0, 3.0);

    auto r1 = v * 2.0;
    EXPECT_NEAR(r1.get_x(), 3.0, 1e-9);
    EXPECT_NEAR(r1.get_y(), -4.0, 1e-9);
    EXPECT_NEAR(r1.get_z(), 6.0, 1e-9);

    auto r2 = v * 0.0;
    EXPECT_NEAR(r2.get_x(), 0.0, 1e-9);
    EXPECT_NEAR(r2.get_y(), 0.0, 1e-9);
    EXPECT_NEAR(r2.get_z(), 0.0, 1e-9);

    auto r3 = v * -1.0;
    EXPECT_NEAR(r3.get_x(), -1.5, 1e-9);
    EXPECT_NEAR(r3.get_y(), 2.0, 1e-9);
    EXPECT_NEAR(r3.get_z(), -3.0, 1e-9);
}

TEST(Vector, scalar_divide) {
    Vector<double> v(3.0, -6.0, 9.0);

    auto r1 = v / 3.0;
    EXPECT_NEAR(r1.get_x(), 1.0, 1e-9);
    EXPECT_NEAR(r1.get_y(), -2.0, 1e-9);
    EXPECT_NEAR(r1.get_z(), 3.0, 1e-9);

    auto r2 = v / -3.0;
    EXPECT_NEAR(r2.get_x(), -1.0, 1e-9);
    EXPECT_NEAR(r2.get_y(), 2.0, 1e-9);
    EXPECT_NEAR(r2.get_z(), -3.0, 1e-9);
}

TEST(Vector, addition) {
    Vector<double> a(1.0, 2.0, 3.0);
    Vector<double> b(4.0, -1.0, 0.5);
    auto r = a + b;

    EXPECT_NEAR(r.get_x(), 5.0, 1e-9);
    EXPECT_NEAR(r.get_y(), 1.0, 1e-9);
    EXPECT_NEAR(r.get_z(), 3.5, 1e-9);
}

TEST(Vector, subtraction) {
    Vector<double> a(1.0, 2.0, 3.0);
    Vector<double> b(4.0, -1.0, 0.5);

    auto r1 = a - b;
    EXPECT_NEAR(r1.get_x(), -3.0, 1e-9);
    EXPECT_NEAR(r1.get_y(), 3.0, 1e-9);
    EXPECT_NEAR(r1.get_z(), 2.5, 1e-9);

    auto r2 = b - a;
    EXPECT_NEAR(r2.get_x(), 3.0, 1e-9);
    EXPECT_NEAR(r2.get_y(), -3.0, 1e-9);
    EXPECT_NEAR(r2.get_z(), -2.5, 1e-9);
}

TEST(Vector, abs) {
    Vector<double> v(3.0, 4.0, 12.0);
    EXPECT_NEAR(v.abs(), 13.0, 1e-12);

    Vector<double> zero(0.0, 0.0, 0.0);
    EXPECT_NEAR(zero.abs(), 0.0, 1e-12);
}

TEST(Vector, normalize) {
    Vector<double> v(3.0, 4.0, 12.0);
    auto n = v.normalize();

    EXPECT_NEAR(n.abs(), 1.0, 1e-12);
    EXPECT_NEAR(n.get_x(), 3.0 / 13.0, 1e-12);
    EXPECT_NEAR(n.get_y(), 4.0 / 13.0, 1e-12);
    EXPECT_NEAR(n.get_z(), 12.0 / 13.0, 1e-12);

    Vector<double> zero(0.0, 0.0, 0.0);
    auto nzero = zero.normalize();
    EXPECT_NEAR(nzero.get_x(), 0.0, 1e-12);
    EXPECT_NEAR(nzero.get_y(), 0.0, 1e-12);
    EXPECT_NEAR(nzero.get_z(), 0.0, 1e-12);
}

TEST(Vector, orthogonal) {
    Point<float>  a(1.f, 1.f, 1.f);
    Point<float>  b(2.f, 1.f, 1.f);
    Point<float>  c(3.f, 2.f, 1.f);
    Point<float>  d(3.f, 6.f, 1.f);
    Vector<float> v1(a, b);
    Vector<float> v2(c, d);

    ASSERT_TRUE(v1.orthogonal(v2));
}

TEST(Vector, collinear) {
    Vector<float> v1(1, 2, 3);
    Vector<float> v2(3, 6, 9);

    ASSERT_TRUE(v1.collinear(v2));
}

TEST(Vector, is_nul) {
    Point<float>  a(1, 1, 1);
    Point<float>  b(2, 1, 1);
    Vector<float> v1(a, b);
    Vector<float> v2(0, 0, 0);

    EXPECT_FALSE(v1.is_nul());
    EXPECT_TRUE(v2.is_nul());
}

TEST(Vector, projection) {
    Vector<float> v1(1, 1, 0);
    Vector<float> u1(2, 0, 0);

    Vector<float> proj1 = v1.projection(u1);

    EXPECT_NEAR(proj1.get_x(), 1.0, 1e-6);
    EXPECT_NEAR(proj1.get_y(), 0.0, 1e-6);
    EXPECT_NEAR(proj1.get_z(), 0.0, 1e-6);

    Vector<float> v2(0, 5, 1);
    Vector<float> u2(0, 1, 0);

    Vector<float> proj2 = v2.projection(u2);

    EXPECT_NEAR(proj2.get_x(), 0.0, 1e-6);
    EXPECT_NEAR(proj2.get_y(), 5.0, 1e-6);
    EXPECT_NEAR(proj2.get_z(), 0.0, 1e-6);

    Vector<float> v3(3, 4, 0);
    Vector<float> u3(0, 0, 1);

    Vector<float> proj3 = v3.projection(u3);

    EXPECT_NEAR(proj3.get_x(), 0.0, 1e-6);
    EXPECT_NEAR(proj3.get_y(), 0.0, 1e-6);
    EXPECT_NEAR(proj3.get_z(), 0.0, 1e-6);
}
