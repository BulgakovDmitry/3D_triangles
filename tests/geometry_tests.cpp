#include <gtest/gtest.h>

#include "geometry.hpp"

// --------------------------------------------------------------------------------------
//                           Tests class Point
// --------------------------------------------------------------------------------------

TEST(points, equal_points) {
    // arrange
    Point a(1, 1, 1);
    Point b(2, 1, 1);
    Point c(1, 1, 1);

    // act, assert
    EXPECT_FALSE(a.equal(b));
    EXPECT_TRUE(a.equal(c));
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

// --------------------------------------------------------------------------------------
//                           Tests class Interval
// --------------------------------------------------------------------------------------

TEST(interval, empty_points) {
    // arrange
    Line l(Point(0,0,0), Point(1,0,0)); 
    std::vector<Point> pts; 

    // act
    Interval I(l, pts);

    // assert: 
    EXPECT_NEAR(I.get_p_min().get_x(), l.get_r0().get_x(), 1e-6);
    EXPECT_NEAR(I.get_p_min().get_y(), l.get_r0().get_y(), 1e-6);
    EXPECT_NEAR(I.get_p_min().get_z(), l.get_r0().get_z(), 1e-6);

    EXPECT_NEAR(I.get_p_max().get_x(), l.get_r0().get_x(), 1e-6);
    EXPECT_NEAR(I.get_p_max().get_y(), l.get_r0().get_y(), 1e-6);
    EXPECT_NEAR(I.get_p_max().get_z(), l.get_r0().get_z(), 1e-6);
}

TEST(interval, points_on_line) {
    // arrange
    Line l(Point(0,0,0), Point(1,0,0)); 
    std::vector<Point> pts = { Point(2,0,0), Point(-3,0,0), Point(1,0,0) };

    // act
    Interval I(l, pts);

    // assert
    EXPECT_NEAR(I.get_p_min().get_x(), -3.0, 1e-6);
    EXPECT_NEAR(I.get_p_min().get_y(), 0.0, 1e-6);
    EXPECT_NEAR(I.get_p_min().get_z(), 0.0, 1e-6);

    EXPECT_NEAR(I.get_p_max().get_x(), 2.0, 1e-6);
    EXPECT_NEAR(I.get_p_max().get_y(), 0.0, 1e-6);
    EXPECT_NEAR(I.get_p_max().get_z(), 0.0, 1e-6);
}

TEST(interval, points_off_line) {
    // arrange
    Line l(Point(0,0,0), Point(1,0,0));
    std::vector<Point> pts = { Point(3,4,0), Point(-1,2,5) };

    // act
    Interval I(l, pts);

    // assert
    EXPECT_NEAR(I.get_p_min().get_x(), -1.0, 1e-6);
    EXPECT_NEAR(I.get_p_min().get_y(), 0.0, 1e-6);
    EXPECT_NEAR(I.get_p_min().get_z(), 0.0, 1e-6);

    EXPECT_NEAR(I.get_p_max().get_x(), 3.0, 1e-6);
    EXPECT_NEAR(I.get_p_max().get_y(), 0.0, 1e-6);
    EXPECT_NEAR(I.get_p_max().get_z(), 0.0, 1e-6);
}

TEST(interval, zero_direction_vector_throws) {
    // arrange
    Line l(Point(0,0,0), Point(0,0,0)); 
    std::vector<Point> pts = { Point(1,2,3) };

    // act, assert
    EXPECT_THROW(Interval I(l, pts), std::runtime_error);
}

TEST(interval, valid) {
    // arrange
    Line l(Point(0,0,0), Point(1,0,0));
    Point p1(3, 1, 0);
    Point p2(7, 0, 1);

    // act
    Interval I1(l, {p1, p2});
    Interval I2(l, {p1, p2});
    I2.erase();

    // assert
    EXPECT_TRUE(I1.valid());
    EXPECT_FALSE(I2.valid());
}

// --------------------------------------------------------------------------------------
//                           Tests class Polygon
// --------------------------------------------------------------------------------------

TEST(polygons, valid_polygons) {
    // arrange
    Point  a(1, 1, 1);
    Point  b(2, 7, 1);
    Point  c(3, 2, 1);
    Point  d(3, 6, 1);

    Polygon pol1({a, b, c, d});
    Polygon pol2({a, b, c, d});

    // act
    pol2.erase();
    
    // assert
    EXPECT_TRUE(pol1.valid());
    EXPECT_FALSE(pol2.valid());
}