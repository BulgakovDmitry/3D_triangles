#include <gtest/gtest.h>

#include "triangle.hpp"
#include "triangle_to_triangle.hpp"
#include "triangle_to_triangle_2d.hpp"
#include "point.hpp"

using namespace triangle;
using namespace intersection_3d;
using namespace intersection_2d;

// --------------------------------------------------------------------------------------
//                           Tests intersect_3d
// --------------------------------------------------------------------------------------
TEST(intersect_3d, triangle) {
    // arrange
    Triangle<float> tr1(Point<float>(0, 0, 2), Point<float>(0, 1, 0), Point<float>(1, 0, 0));
    Triangle<float> tr2(Point<float>(0, 0, 1), Point<float>(0, 2, 0), Point<float>(2, 0, 0));
    Triangle<float> tr3(Point<float>(0, 0, 3), Point<float>(0, 2, 0), Point<float>(2, 0, 0));

    // act, assert
    EXPECT_TRUE(intersect(tr1, tr2));
    EXPECT_FALSE(intersect(tr1, tr3));
}

TEST(intersect_3d, PiercesInteriorAtPoint) {
    // arrange
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(0.5,0.5,-1), Point<float>(0.5,0.5,1), Point<float>(2,2,2));

    // act, assert
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, TouchesInsideAtVertex) {
    // arrange
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(0.3,0.3,0), Point<float>(2,0,1), Point<float>(0,2,1));

    // act, assert
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, EdgeInPlaneOverlapsTriangle) {
    // arrange
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(0.2,0.2,0), Point<float>(1.2,0.2,0), Point<float>(0.5,0.5,1));

    // act, assert
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, PointInPlaneOverlapsTriangleIntersection) {
    // arrange
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(1,0,0), Point<float>(0,1,0));
    Triangle<float> t2(Point<float>(0,0.5,-0.5), Point<float>(0,0.5,0.5), Point<float>(-1,0,0));

    // act, assert
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, CrossesPlaneOutside_NoIntersection) {
    // arrange
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(3,3,-1), Point<float>(3,3,1), Point<float>(4,4,0));

    // act, assert
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, ParallelPlanes_NoIntersection) {
    // arrange
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(0,0,1), Point<float>(2,0,1), Point<float>(0,2,1));

    // act, assert
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, TouchesAtT1Vertex) {
    // arrange
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(0,0,-1), Point<float>(0,0,1), Point<float>(1,1,1));

    // act, assert
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Degenerate_CollinearSegment_PiercesTriangle) {
    // t1 in the plane z=0; tSeg are collinear points: a segment through the interior of t1
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> tSeg(Point<float>(0.5,0.5,-1), Point<float>(0.5,0.5,0), Point<float>(0.5,0.5,1));
    EXPECT_TRUE(intersect(t1, tSeg));
    EXPECT_TRUE(intersect(tSeg, t1));
}

TEST(intersect_3d, Degenerate_CollinearSegment_Coplanar_OverlapsEdge) {
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> tSeg(Point<float>(0,0,0), Point<float>(1,0,0), Point<float>(2,0,0)); // segment on edge t1
    EXPECT_TRUE(intersect(t1, tSeg));
    EXPECT_TRUE(intersect(tSeg, t1));
}

TEST(intersect_3d, Degenerate_CollinearSegment_ParallelOffset_NoIntersection) {
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> tSeg(Point<float>(3,3,1), Point<float>(4,4,1), Point<float>(5,5,1));
    EXPECT_FALSE(intersect(t1, tSeg));
    EXPECT_FALSE(intersect(tSeg, t1));
}

TEST(intersect_3d, Degenerate_Segment_Triangle_Intersection) {
    Triangle<float> tSeg(Point<float>(0,0,0), Point<float>(0,0,0), Point<float>(4,4,4));
    Triangle<float> t1(Point<float>(0,0,1), Point<float>(0,1,0), Point<float>(1,0,0));
    EXPECT_TRUE(intersect(t1, tSeg));
    EXPECT_TRUE(intersect(tSeg, t1));
}

TEST(intersect_3d, Degenerate_Point_InsideTriangle) {
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> tPt(Point<float>(0.3,0.3,0), Point<float>(0.3,0.3,0), Point<float>(0.3,0.3,0));
    EXPECT_TRUE(intersect(t1, tPt));
    EXPECT_TRUE(intersect(tPt, t1));
}

TEST(intersect_3d, Degenerate_Point_AtVertex) {
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> tPt(Point<float>(0,0,0), Point<float>(0,0,0), Point<float>(0,0,0));
    EXPECT_TRUE(intersect(t1, tPt));
    EXPECT_TRUE(intersect(tPt, t1));
}

TEST(intersect_3d, Degenerate_Point_OffPlane_NoIntersection) {
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> tPt(Point<float>(0.3,0.3,0.1), Point<float>(0.3,0.3,0.1), Point<float>(0.3,0.3,0.1));
    EXPECT_FALSE(intersect(t1, tPt));
    EXPECT_FALSE(intersect(tPt, t1));
}

TEST(intersect_3d, Degenerate_Segment_Segment_NoIntersection) {
    Triangle<float> t1(Point<float>(2,0,0), Point<float>(1,0,0), Point<float>(0,0,0));
    Triangle<float> t2(Point<float>(0,0,1), Point<float>(0,0,2), Point<float>(0,0,3));
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, Degenerate_Segment_Segment_Intersection) {
    Triangle<float> t1(Point<float>(2,0,0), Point<float>(1,0,0), Point<float>(0,0,0));
    Triangle<float> t2(Point<float>(1,0,-1), Point<float>(1,0,0), Point<float>(1,0,1));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Degenerate_Point_Segment_NoIntersection) {
    Triangle<float> t1(Point<float>(2,0,0), Point<float>(1,0,0), Point<float>(0,0,0));
    Triangle<float> t2(Point<float>(0,0,1), Point<float>(0,0,1), Point<float>(0,0,1));
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, Degenerate_Point_Segment_Intersection) {
    Triangle<float> t1(Point<float>(0,0,2), Point<float>(0,0,3), Point<float>(0,0,-1));
    Triangle<float> t2(Point<float>(0,0,1), Point<float>(0,0,1), Point<float>(0,0,1));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Degenerate_Two_Points_NoIntersection) {
    Triangle<float> t1(Point<float>(2,0,0), Point<float>(2,0,0), Point<float>(2,0,0));
    Triangle<float> t2(Point<float>(0,0,1), Point<float>(0,0,1), Point<float>(0,0,1));
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, Degenerate_Two_Points_Intersection) {
    Triangle<float> t1(Point<float>(2,0,0), Point<float>(2,0,0), Point<float>(2,0,0));
    Triangle<float> t2(Point<float>(2,0,0), Point<float>(2,0,0), Point<float>(2,0,0));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_Disjoint_NoIntersection) {
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(3,0,0), Point<float>(5,0,0), Point<float>(3,2,0));
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_ShareFullEdge_Touching) {
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(2,0,0), Point<float>(0,2,0), Point<float>(2,2,0)); // common edge
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_Containment_T2InsideT1) {
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(3,0,0), Point<float>(0,3,0));
    Triangle<float> t2(Point<float>(0.5,0.5,0), Point<float>(1.5,0.5,0), Point<float>(0.5,1.5,0));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_TouchAtSingleVertexOnly) {
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(2,0,0), Point<float>(3,0,0), Point<float>(2,1,0)); // touching at the top (2,0,0)
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_IdenticalTriangles) {
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, SkewPlanes_IntersectInSegment) {
    // the planes are not parallel; the intersection line gives a segment inside both
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(0.2,0.2,-1), Point<float>(1.5,0.2,1), Point<float>(0.2,1.5,1));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, SkewPlanes_LineCutsOutside_NoIntersection) {
    // t2 intersects plane t1, but the section is outside t1
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(3,3,-1), Point<float>(3,3,1), Point<float>(4,2,0.5));
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, SkewPlanes_InteriorSegment_Diagonal) {
    // Two non-parallel planes, the intersection is a segment inside both triangles
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(3,0,0), Point<float>(0,3,0));
    Triangle<float> t2(Point<float>(0.3,0.3,-1), Point<float>(2.4,0.3,1), Point<float>(0.3,2.4,1));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, SkewPlanes_InteriorSegment_TouchesT1Edges) {
    // The intersection segment lies from (1,0,0) to (0,1,0) - touches the edges t1
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(0,0,-1), Point<float>(2,0,1), Point<float>(0,2,1));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, EdgeFace_Pierce_InteriorPoint) {
    // Edge t2 pierces the interior of t1 at one point
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(0.8,0.6,-1), Point<float>(0.8,0.6,1), Point<float>(3,-1,0.5));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, VertexOnT1Edge_PointContact) {
    // Vertex t2 lies on edge t1, the remaining points are outside the plane t1
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(1,0,0), Point<float>(1,0,1), Point<float>(2,1,1));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, NonCoplanar_ShareFullEdge) {
    // Common full edge for non-parallel planes (intersection line = edge)
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(1,0,1)); // плоскость y=0
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, SkewPlanes_LineHitsOutside_NoIntersection_2) {
    // The intersection line of the planes passes outside t1
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(0,2,0));
    Triangle<float> t2(Point<float>(3,3,-1), Point<float>(5,3,1), Point<float>(3,5,1));
    EXPECT_FALSE(intersect(t1, t2));
    EXPECT_FALSE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_PartialAreaOverlap) {
    // Coplanar triangles with non-empty intersection area
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(3,0,0), Point<float>(0,3,0));
    Triangle<float> t2(Point<float>(0.5,0,0), Point<float>(2.5,0,0), Point<float>(0.5,2,0));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

TEST(intersect_3d, Coplanar_ProperEdgeCross) {
    // Coplanar: edges intersect, no vertex lies inside another
    Triangle<float> t1(Point<float>(0,0,0), Point<float>(2,0,0), Point<float>(1,2,0));
    Triangle<float> t2(Point<float>(-0.5,0.5,0), Point<float>(2.5,0.5,0), Point<float>(1.5,-1,0));
    EXPECT_TRUE(intersect(t1, t2));
    EXPECT_TRUE(intersect(t2, t1));
}

// --------------------------------------------------------------------------------------
//                           Tests intersect_2d
// --------------------------------------------------------------------------------------
static Point<float> P(float x, float y, float z = 0.0) { return Point<float>{x, y, z}; }

TEST(Intersect2D, Disjoint_NoOverlap) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(3,3), P(4,3), P(3,4) };

    // act, assert
    EXPECT_FALSE(intersect_2d(A, B));
    EXPECT_FALSE(intersect_2d(B, A));
}

TEST(Intersect2D, Containment_B_inside_A) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(0.2,0.2), P(0.6,0.2), P(0.2,0.6) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(Intersect2D, Containment_OrientationDoesNotMatter) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(0.2,0.6), P(0.6,0.2), P(0.2,0.2) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(Intersect2D, EdgeEdge_ProperCross_NoVertexInside) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(1,2) };
    Triangle<float> B{ P(-0.5,0.5), P(2.5,0.5), P(1.5,-1) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(Intersect2D, Touch_VertexOnEdge_PointContact) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(1,0), P(1.1,-0.2), P(0.9,-0.2) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(Intersect2D, Collinear_OverlappingEdges) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(0,0), P(2,0), P(3,1) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(Intersect2D, Touch_AtSingleSharedVertexOnly) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(2,0), P(3,0), P(2,1) };

    // act, assert
    EXPECT_TRUE(intersect_2d(A, B));
    EXPECT_TRUE(intersect_2d(B, A));
}

TEST(Intersect2D, Disjoint_CloseButSeparated) {
    // arrange
    Triangle<float> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<float> B{ P(2.1,0.0), P(3.0,0.0), P(2.1,0.9) };

    // act, assert
    EXPECT_FALSE(intersect_2d(A, B));
    EXPECT_FALSE(intersect_2d(B, A));
}
