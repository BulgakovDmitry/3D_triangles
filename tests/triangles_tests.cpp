#include <gtest/gtest.h>

#include "primitives/triangle.hpp"
#include "intersection/triangle_to_triangle.hpp"
#include "intersection/triangle_to_triangle_2d.hpp"
#include "primitives/point.hpp"

using namespace triangle;
using namespace intersection_3d;
using namespace intersection_2d;

// --------------------------------------------------------------------------------------
//                           Tests intersect_3d
// --------------------------------------------------------------------------------------
TEST(intersect_3d, triangle) {
    // arrange
    Triangle tr1(Point(0, 0, 2), Point(0, 1, 0), Point(1, 0, 0));
    Triangle tr2(Point(0, 0, 1), Point(0, 2, 0), Point(2, 0, 0));
    Triangle tr3(Point(0, 0, 3), Point(0, 2, 0), Point(2, 0, 0));

    // act, assert
    EXPECT_TRUE(intersect(tr1, tr2));
    EXPECT_FALSE(intersect(tr1, tr3));
}

TEST(intersect_3d, PiercesInteriorAtPoint) {
    // arrange
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0.5,0.5,-1), Point(0.5,0.5,1), Point(2,2,2));

    // act, assert
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, TouchesInsideAtVertex) {
    // arrange
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0.3,0.3,0), Point(2,0,1), Point(0,2,1));

    // act, assert
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, EdgeInPlaneOverlapsTriangle) {
    // arrange
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0.2,0.2,0), Point(1.2,0.2,0), Point(0.5,0.5,1));

    // act, assert
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, CrossesPlaneOutside_NoIntersection) {
    // arrange
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(3,3,-1), Point(3,3,1), Point(4,4,0));

    // act, assert
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, ParallelPlanes_NoIntersection) {
    // arrange
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0,0,1), Point(2,0,1), Point(0,2,1));

    // act, assert
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, TouchesAtT1Vertex) {
    // arrange
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0,0,-1), Point(0,0,1), Point(1,1,1));

    // act, assert
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Degenerate_CollinearSegment_PiercesTriangle) {
    // t1 in the plane z=0; tSeg are collinear points: a segment through the interior of t1
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle tSeg(Point(0.5,0.5,-1), Point(0.5,0.5,0), Point(0.5,0.5,1));
    EXPECT_TRUE(intersect(t1, tSeg));
    EXPECT_TRUE(intersect(tSeg, t1));
}

TEST(intersect_3d, Degenerate_CollinearSegment_Coplanar_OverlapsEdge) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle tSeg(Point(0,0,0), Point(1,0,0), Point(2,0,0)); // segment on edge t1
    EXPECT_TRUE(intersect(t1, tSeg));
    EXPECT_TRUE(intersect(tSeg, t1));
}

TEST(intersect_3d, Degenerate_CollinearSegment_ParallelOffset_NoIntersection) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle tSeg(Point(3,3,1), Point(4,4,1), Point(5,5,1));
    EXPECT_FALSE(intersect(t1, tSeg));
    EXPECT_FALSE(intersect(tSeg, t1));
}

TEST(intersect_3d, Degenerate_Point_InsideTriangle) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle tPt(Point(0.3,0.3,0), Point(0.3,0.3,0), Point(0.3,0.3,0));
    EXPECT_TRUE(intersect(t1, tPt));
    EXPECT_TRUE(intersect(tPt, t1));
}

TEST(intersect_3d, Degenerate_Point_AtVertex) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle tPt(Point(0,0,0), Point(0,0,0), Point(0,0,0));
    EXPECT_TRUE(intersect(t1, tPt));
    EXPECT_TRUE(intersect(tPt, t1));
}

TEST(intersect_3d, Degenerate_Point_OffPlane_NoIntersection) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle tPt(Point(0.3,0.3,0.1), Point(0.3,0.3,0.1), Point(0.3,0.3,0.1));
    EXPECT_FALSE(intersect(t1, tPt));
    EXPECT_FALSE(intersect(tPt, t1));
}

TEST(intersect_3d, Degenerate_Segment_Segment_NoIntersection) {
    Triangle t1(Point(2,0,0), Point(1,0,0), Point(0,0,0));
    Triangle t2(Point(0,0,1), Point(0,0,2), Point(0,0,3));
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, Degenerate_Segment_Segment_Intersection) {
    Triangle t1(Point(2,0,0), Point(1,0,0), Point(0,0,0));
    Triangle t2(Point(1,0,-1), Point(1,0,0), Point(1,0,1));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Degenerate_Point_Segment_NoIntersection) {
    Triangle t1(Point(2,0,0), Point(1,0,0), Point(0,0,0));
    Triangle t2(Point(0,0,1), Point(0,0,1), Point(0,0,1));
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, Degenerate_Point_Segment_Intersection) {
    Triangle t1(Point(0,0,2), Point(0,0,3), Point(0,0,-1));
    Triangle t2(Point(0,0,1), Point(0,0,1), Point(0,0,1));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Degenerate_Two_Points_NoIntersection) {
    Triangle t1(Point(2,0,0), Point(2,0,0), Point(2,0,0));
    Triangle t2(Point(0,0,1), Point(0,0,1), Point(0,0,1));
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, Degenerate_Two_Points_Intersection) {
    Triangle t1(Point(2,0,0), Point(2,0,0), Point(2,0,0));
    Triangle t2(Point(2,0,0), Point(2,0,0), Point(2,0,0));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_Disjoint_NoIntersection) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(3,0,0), Point(5,0,0), Point(3,2,0));
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_ShareFullEdge_Touching) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(2,0,0), Point(0,2,0), Point(2,2,0)); // common edge
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_Containment_T2InsideT1) {
    Triangle t1(Point(0,0,0), Point(3,0,0), Point(0,3,0));
    Triangle t2(Point(0.5,0.5,0), Point(1.5,0.5,0), Point(0.5,1.5,0));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_TouchAtSingleVertexOnly) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(2,0,0), Point(3,0,0), Point(2,1,0)); // touching at the top (2,0,0)
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_IdenticalTriangles) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, SkewPlanes_IntersectInSegment) {
    // the planes are not parallel; the intersection line gives a segment inside both
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0.2,0.2,-1), Point(1.5,0.2,1), Point(0.2,1.5,1));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, SkewPlanes_LineCutsOutside_NoIntersection) {
    // t2 intersects plane t1, but the section is outside t1
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(3,3,-1), Point(3,3,1), Point(4,2,0.5));
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, SkewPlanes_InteriorSegment_Diagonal) {
    // Two non-parallel planes, the intersection is a segment inside both triangles
    Triangle t1(Point(0,0,0), Point(3,0,0), Point(0,3,0));
    Triangle t2(Point(0.3,0.3,-1), Point(2.4,0.3,1), Point(0.3,2.4,1));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, SkewPlanes_InteriorSegment_TouchesT1Edges) {
    // The intersection segment lies from (1,0,0) to (0,1,0) - touches the edges t1
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0,0,-1), Point(2,0,1), Point(0,2,1));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, EdgeFace_Pierce_InteriorPoint) {
    // Edge t2 pierces the interior of t1 at one point
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0.8,0.6,-1), Point(0.8,0.6,1), Point(3,-1,0.5));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, VertexOnT1Edge_PointContact) {
    // Vertex t2 lies on edge t1, the remaining points are outside the plane t1
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(1,0,0), Point(1,0,1), Point(2,1,1));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, NonCoplanar_ShareFullEdge) {
    // Common full edge for non-parallel planes (intersection line = edge)
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0,0,0), Point(2,0,0), Point(1,0,1)); // плоскость y=0
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, SkewPlanes_LineHitsOutside_NoIntersection_2) {
    // The intersection line of the planes passes outside t1
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(3,3,-1), Point(5,3,1), Point(3,5,1));
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_PartialAreaOverlap) {
    // Coplanar triangles with non-empty intersection area
    Triangle t1(Point(0,0,0), Point(3,0,0), Point(0,3,0));
    Triangle t2(Point(0.5,0,0), Point(2.5,0,0), Point(0.5,2,0));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_ProperEdgeCross) {
    // Coplanar: edges intersect, no vertex lies inside another
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(1,2,0));
    Triangle t2(Point(-0.5,0.5,0), Point(2.5,0.5,0), Point(1.5,-1,0));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

// --------------------------------------------------------------------------------------
//                           Tests intersect_2d
// --------------------------------------------------------------------------------------
static Point P(float x, float y, float z = 0.0) { return Point{x, y, z}; }

TEST(Intersect2D, Disjoint_NoOverlap) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(3,3), P(4,3), P(3,4) };

    // act, assert
    EXPECT_FALSE(intersect_2d(A, B));
    EXPECT_FALSE(intersect_2d(B, A));
}

TEST(Intersect2D, Containment_B_inside_A) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(0.2,0.2), P(0.6,0.2), P(0.2,0.6) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(Intersect2D, Containment_OrientationDoesNotMatter) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(0.2,0.6), P(0.6,0.2), P(0.2,0.2) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(Intersect2D, EdgeEdge_ProperCross_NoVertexInside) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(1,2) };
    Triangle B{ P(-0.5,0.5), P(2.5,0.5), P(1.5,-1) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(Intersect2D, Touch_VertexOnEdge_PointContact) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(1,0), P(1.1,-0.2), P(0.9,-0.2) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(Intersect2D, Collinear_OverlappingEdges) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(0,0), P(2,0), P(3,1) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(Intersect2D, Touch_AtSingleSharedVertexOnly) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(2,0), P(3,0), P(2,1) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(Intersect2D, Disjoint_CloseButSeparated) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(2.1,0.0), P(3.0,0.0), P(2.1,0.9) };

    // act, assert
    EXPECT_FALSE(intersect_2d(A, B));
    EXPECT_FALSE(intersect_2d(B, A));
}
