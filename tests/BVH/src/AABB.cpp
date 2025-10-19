#include <gtest/gtest.h>
#include <limits>

#include "AABB.hpp"
#include "point.hpp"

using namespace triangle;
using AABBd = bounding_box::AABB<double>;
using P     = Point<double>;

static AABBd make_box(double x1,double y1,double z1,double x2,double y2,double z2){
    return AABBd(P{x1,y1,z1}, P{x2,y2,z2});
}

// -------------------------------- Constructors --------------------------------

TEST(AABB, DefaultConstructorInitializesToExtremes) {
    AABBd aabb;

    EXPECT_DOUBLE_EQ(aabb.p_min.x_, std::numeric_limits<double>::max());
    EXPECT_DOUBLE_EQ(aabb.p_min.y_, std::numeric_limits<double>::max());
    EXPECT_DOUBLE_EQ(aabb.p_min.z_, std::numeric_limits<double>::max());

    EXPECT_DOUBLE_EQ(aabb.p_max.x_, std::numeric_limits<double>::lowest());
    EXPECT_DOUBLE_EQ(aabb.p_max.y_, std::numeric_limits<double>::lowest());
    EXPECT_DOUBLE_EQ(aabb.p_max.z_, std::numeric_limits<double>::lowest());
}

TEST(AABB, ParameterizedConstructorStoresMinMax) {
    P minp(1,2,3), maxp(4,5,6);
    AABBd aabb(minp, maxp);

    EXPECT_DOUBLE_EQ(aabb.p_min.x_, 1);
    EXPECT_DOUBLE_EQ(aabb.p_min.y_, 2);
    EXPECT_DOUBLE_EQ(aabb.p_min.z_, 3);

    EXPECT_DOUBLE_EQ(aabb.p_max.x_, 4);
    EXPECT_DOUBLE_EQ(aabb.p_max.y_, 5);
    EXPECT_DOUBLE_EQ(aabb.p_max.z_, 6);
}

TEST(AABB, ConstructorWithReversedCoordinates) {
    AABBd aabb(P{5,5,5}, P{1,1,1});
    EXPECT_DOUBLE_EQ(aabb.p_min.x_, 5);
    EXPECT_DOUBLE_EQ(aabb.p_max.x_, 1);
}

// ---------------------------------- Expand ------------------------------------

TEST(AABB, ExpandWithLargerAABBIncreasesBounds) {
    AABBd original = make_box(0,0,0, 1,1,1);
    AABBd larger   = make_box(-1,-1,-1, 2,2,2);

    original.wrap_in_box_with(larger);

    EXPECT_DOUBLE_EQ(original.p_min.x_, -1);
    EXPECT_DOUBLE_EQ(original.p_min.y_, -1);
    EXPECT_DOUBLE_EQ(original.p_min.z_, -1);

    EXPECT_DOUBLE_EQ(original.p_max.x_, 2);
    EXPECT_DOUBLE_EQ(original.p_max.y_, 2);
    EXPECT_DOUBLE_EQ(original.p_max.z_, 2);
}

TEST(AABB, ExpandWithSmallerAABBNoChange) {
    AABBd original = make_box(0,0,0, 3,3,3);
    AABBd smaller  = make_box(1,1,1, 2,2,2);

    original.wrap_in_box_with(smaller);

    EXPECT_DOUBLE_EQ(original.p_min.x_, 0);
    EXPECT_DOUBLE_EQ(original.p_max.x_, 3);
}

TEST(AABB, ExpandWithPartialOverlapExtendsAppropriately) {
    AABBd original = make_box(0,0,0, 2,2,2);
    AABBd other    = make_box(1,1,1, 3,3,3);

    original.wrap_in_box_with(other);

    EXPECT_DOUBLE_EQ(original.p_min.x_, 0);
    EXPECT_DOUBLE_EQ(original.p_max.x_, 3);
}

TEST(AABB, ExpandWithNegativeCoordinates) {
    AABBd original = make_box(0,0,0, 1,1,1);
    AABBd negative = make_box(-2,-2,-2, -1,-1,-1);

    original.wrap_in_box_with(negative);

    EXPECT_DOUBLE_EQ(original.p_min.x_, -2);
    EXPECT_DOUBLE_EQ(original.p_max.x_, 1);
}

TEST(AABB, ExpandEmptyAABBWithNormal) {
    AABBd empty;
    AABBd normal = make_box(0,0,0, 1,1,1);

    empty.wrap_in_box_with(normal);

    EXPECT_DOUBLE_EQ(empty.p_min.x_, 0);
    EXPECT_DOUBLE_EQ(empty.p_max.x_, 1);
}

TEST(AABB, ExpandNormalWithEmptyAABB) {
    AABBd normal = make_box(0,0,0, 1,1,1);
    AABBd empty;

    normal.wrap_in_box_with(empty);

    EXPECT_DOUBLE_EQ(normal.p_min.x_, 0);
    EXPECT_DOUBLE_EQ(normal.p_max.x_, 1);
}

TEST(AABB, ExpandMultipleTimes) {
    AABBd aabb = make_box(0,0,0, 1,1,1);

    aabb.wrap_in_box_with(make_box(-1,2,3, 4,5,6));
    aabb.wrap_in_box_with(make_box(7,-2,1, 8,0,2));

    EXPECT_DOUBLE_EQ(aabb.p_min.x_, -1);
    EXPECT_DOUBLE_EQ(aabb.p_min.y_, -2);
    EXPECT_DOUBLE_EQ(aabb.p_min.z_, 0);

    EXPECT_DOUBLE_EQ(aabb.p_max.x_, 8);
    EXPECT_DOUBLE_EQ(aabb.p_max.y_, 5);
    EXPECT_DOUBLE_EQ(aabb.p_max.z_, 6);
}

// -------------------------------- GetCenter -----------------------------------

TEST(AABB, GetCenterBasic) {
    AABBd aabb = make_box(0,0,0, 1,1,1);
    P center = aabb.get_center();
    EXPECT_DOUBLE_EQ(center.x_, 0.5);
    EXPECT_DOUBLE_EQ(center.y_, 0.5);
    EXPECT_DOUBLE_EQ(center.z_, 0.5);
}

TEST(AABB, GetCenterSymmetricNegative) {
    AABBd aabb = make_box(-2,-2,-2, 2,2,2);
    P center = aabb.get_center();
    EXPECT_DOUBLE_EQ(center.x_, 0);
    EXPECT_DOUBLE_EQ(center.y_, 0);
    EXPECT_DOUBLE_EQ(center.z_, 0);
}

TEST(AABB, GetCenterAsymmetric) {
    AABBd aabb = make_box(1,2,3, 5,6,7);
    P center = aabb.get_center();
    EXPECT_DOUBLE_EQ(center.x_, 3.0);
    EXPECT_DOUBLE_EQ(center.y_, 4.0);
    EXPECT_DOUBLE_EQ(center.z_, 5.0);
}

TEST(AABB, GetCenterNegativeRange) {
    AABBd aabb = make_box(-5,-3,-1, -1,-1,0);
    P center = aabb.get_center();
    EXPECT_DOUBLE_EQ(center.x_, -3.0);
    EXPECT_DOUBLE_EQ(center.y_, -2.0);
    EXPECT_DOUBLE_EQ(center.z_, -0.5);
}

TEST(AABB, GetCenterDegenerate) {
    AABBd aabb = make_box(1,1,1, 1,1,1);
    P center = aabb.get_center();
    EXPECT_DOUBLE_EQ(center.x_, 1);
    EXPECT_DOUBLE_EQ(center.y_, 1);
    EXPECT_DOUBLE_EQ(center.z_, 1);
}

// -------------------------------- Intersects ----------------------------------

TEST(AABB, IntersectsSeparatedAABB) {
    AABBd aabb1 = make_box(0,0,0, 1,1,1);
    AABBd aabb2 = make_box(2,2,2, 3,3,3);
    EXPECT_FALSE(AABBd::intersect(aabb1, aabb2));
}

TEST(AABB, IntersectsOverlappingAABB) {
    AABBd aabb1 = make_box(0,0,0, 1,1,1);
    AABBd aabb_overlap = make_box(0.5,0.5,0.5, 1.5,1.5,1.5);
    EXPECT_TRUE(AABBd::intersect(aabb1, aabb_overlap));
}

TEST(AABB, IntersectsTouchingAABB) {
    AABBd aabb1 = make_box(0,0,0, 1,1,1);
    AABBd aabb_touching = make_box(1,1,1, 2,2,2);
    EXPECT_TRUE(AABBd::intersect(aabb1, aabb_touching));
}

TEST(AABB, IntersectsIdenticalAABB) {
    AABBd aabb1 = make_box(0,0,0, 1,1,1);
    EXPECT_TRUE(AABBd::intersect(aabb1, aabb1));
}

TEST(AABB, IntersectsContainedAABB) {
    AABBd aabb1 = make_box(0,0,0, 1,1,1);
    AABBd small = make_box(0.25,0.25,0.25, 0.75,0.75,0.75);
    EXPECT_TRUE(AABBd::intersect(aabb1, small));
    EXPECT_TRUE(AABBd::intersect(small, aabb1));
}

TEST(AABB, IntersectsNegativeCoordinates) {
    AABBd neg1 = make_box(-3,-3,-3, -2,-2,-2);
    AABBd neg2 = make_box(-2.5,-2.5,-2.5, -1,-1,-1);
    EXPECT_TRUE(AABBd::intersect(neg1, neg2));
}

TEST(AABB, IntersectsNoOverlapOnSomeAxes) {
    AABBd aabb1 = make_box(0,0,0, 1,1,1);
    AABBd x_overlap = make_box(0.5,5,5, 1.5,6,6);
    EXPECT_FALSE(AABBd::intersect(aabb1, x_overlap));
}

TEST(AABB, IntersectsEdgeTouching) {
    AABBd aabb1 = make_box(0,0,0, 1,1,1);
    AABBd right_touch = make_box(1,0.25,0.25, 2,0.75,0.75);
    EXPECT_TRUE(AABBd::intersect(aabb1, right_touch));
}

TEST(AABB, IntersectsDegenerateAABB) {
    AABBd aabb1 = make_box(0,0,0, 1,1,1);

    AABBd point_inside = make_box(0.5,0.5,0.5, 0.5,0.5,0.5);
    EXPECT_TRUE(AABBd::intersect(aabb1, point_inside));

    AABBd point_outside = make_box(5,5,5, 5,5,5);
    EXPECT_FALSE(AABBd::intersect(aabb1, point_outside));
}

TEST(AABB, IntersectsLargeContainsSmall) {
    AABBd aabb_large = make_box(-10,-10,-10, 10,10,10);
    AABBd aabb1      = make_box(0,0,0, 1,1,1);
    EXPECT_TRUE(AABBd::intersect(aabb_large, aabb1));
    EXPECT_TRUE(AABBd::intersect(aabb1, aabb_large));
}

TEST(AABB, IntersectsCommutative) {
    AABBd aabb1 = make_box(0,0,0, 1,1,1);
    AABBd aabb2 = make_box(2,2,2, 3,3,3);
    AABBd aabb_overlap = make_box(0.5,0.5,0.5, 1.5,1.5,1.5);

    EXPECT_EQ(AABBd::intersect(aabb1, aabb2), AABBd::intersect(aabb2, aabb1));
    EXPECT_EQ(AABBd::intersect(aabb1, aabb_overlap), AABBd::intersect(aabb_overlap, aabb1));
}

// ------------------------------- Special cases --------------------------------

TEST(AABB, DegenerateAABBBehavior) {
    AABBd degenerate = make_box(1,1,1, 1,1,1);

    P center = degenerate.get_center();
    EXPECT_DOUBLE_EQ(center.x_, 1);

    EXPECT_TRUE(AABBd::intersect(degenerate, degenerate));

    AABBd aabb_large = make_box(-10,-10,-10, 10,10,10);
    EXPECT_TRUE(AABBd::intersect(aabb_large, degenerate));
}

TEST(AABB, VeryLargeCoordinates) {
    AABBd large = make_box(-1e6,-1e6,-1e6, 1e6,1e6,1e6);

    P center = large.get_center();
    EXPECT_DOUBLE_EQ(center.x_, 0);
    EXPECT_DOUBLE_EQ(center.y_, 0);
    EXPECT_DOUBLE_EQ(center.z_, 0);

    AABBd aabb1 = make_box(0,0,0, 1,1,1);
    EXPECT_TRUE(AABBd::intersect(large, aabb1));
}

TEST(AABB, PrecisionHandling) {
    AABBd aabb = make_box(0.1,0.2,0.3, 0.4,0.5,0.6);

    P center = aabb.get_center();
    EXPECT_DOUBLE_EQ(center.x_, 0.25);
    EXPECT_DOUBLE_EQ(center.y_, 0.35);
    EXPECT_DOUBLE_EQ(center.z_, 0.45);
}

TEST(AABB, ExpandWithDegenerate) {
    AABBd normal     = make_box(0,0,0, 1,1,1);
    AABBd degenerate = make_box(2,2,2, 2,2,2);

    normal.wrap_in_box_with(degenerate);

    EXPECT_DOUBLE_EQ(normal.p_min.x_, 0);
    EXPECT_DOUBLE_EQ(normal.p_max.x_, 2);
}
