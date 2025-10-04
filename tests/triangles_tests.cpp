#include <gtest/gtest.h>

#include "triangle.hpp"

using namespace triangle;

// --------------------------------------------------------------------------------------
//                           Tests intersect_3d
// --------------------------------------------------------------------------------------
TEST(intersect_3d, triangle) {
    // arrange
    Triangle tr1(Point(0, 0, 2), Point(0, 1, 0), Point(1, 0, 0));
    Triangle tr2(Point(0, 0, 1), Point(0, 2, 0), Point(2, 0, 0));
    Triangle tr3(Point(0, 0, 3), Point(0, 2, 0), Point(2, 0, 0));

    // act, assert
    EXPECT_TRUE(tr1.intersect(tr2));
    EXPECT_FALSE(tr1.intersect(tr3));
}

TEST(intersect_3d, PiercesInteriorAtPoint) {
    // arrange
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0.5,0.5,-1), Point(0.5,0.5,1), Point(2,2,2));

    // act, assert
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, TouchesInsideAtVertex) {
    // arrange
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0.3,0.3,0), Point(2,0,1), Point(0,2,1));

    // act, assert
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, EdgeInPlaneOverlapsTriangle) {
    // arrange
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0.2,0.2,0), Point(1.2,0.2,0), Point(0.5,0.5,1));

    // act, assert
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, CrossesPlaneOutside_NoIntersection) {
    // arrange
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(3,3,-1), Point(3,3,1), Point(4,4,0));

    // act, assert
    EXPECT_FALSE(t1.intersect(t2));
    EXPECT_FALSE(t2.intersect(t1));
}

TEST(intersect_3d, ParallelPlanes_NoIntersection) {
    // arrange
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0,0,1), Point(2,0,1), Point(0,2,1));

    // act, assert
    EXPECT_FALSE(t1.intersect(t2));
    EXPECT_FALSE(t2.intersect(t1));
}

TEST(intersect_3d, TouchesAtT1Vertex) {
    // arrange
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0,0,-1), Point(0,0,1), Point(1,1,1));

    // act, assert
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, Degenerate_CollinearSegment_PiercesTriangle) {
    // t1 в плоскости z=0; tSeg — коллинеарные точки: отрезок сквозь интерьер t1
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle tSeg(Point(0.5,0.5,-1), Point(0.5,0.5,0), Point(0.5,0.5,1));
    EXPECT_TRUE(t1.intersect(tSeg));
    EXPECT_TRUE(tSeg.intersect(t1));
}

TEST(intersect_3d, Degenerate_CollinearSegment_Coplanar_OverlapsEdge) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle tSeg(Point(0,0,0), Point(1,0,0), Point(2,0,0)); // отрезок по ребру t1
    EXPECT_TRUE(t1.intersect(tSeg));
    EXPECT_TRUE(tSeg.intersect(t1));
}

TEST(intersect_3d, Degenerate_CollinearSegment_ParallelOffset_NoIntersection) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle tSeg(Point(3,3,1), Point(4,4,1), Point(5,5,1));
    EXPECT_FALSE(t1.intersect(tSeg));
    EXPECT_FALSE(tSeg.intersect(t1));
}

TEST(intersect_3d, Degenerate_Point_InsideTriangle) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle tPt(Point(0.3,0.3,0), Point(0.3,0.3,0), Point(0.3,0.3,0));
    EXPECT_TRUE(t1.intersect(tPt));
    EXPECT_TRUE(tPt.intersect(t1));
}

TEST(intersect_3d, Degenerate_Point_AtVertex) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle tPt(Point(0,0,0), Point(0,0,0), Point(0,0,0));
    EXPECT_TRUE(t1.intersect(tPt));
    EXPECT_TRUE(tPt.intersect(t1));
}

TEST(intersect_3d, Degenerate_Point_OffPlane_NoIntersection) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle tPt(Point(0.3,0.3,0.1), Point(0.3,0.3,0.1), Point(0.3,0.3,0.1));
    EXPECT_FALSE(t1.intersect(tPt));
    EXPECT_FALSE(tPt.intersect(t1));
}

TEST(intersect_3d, Degenerate_Point_Segment_NoIntersection) {
    Triangle t1(Point(2,0,0), Point(1,0,0), Point(0,0,0));
    Triangle t2(Point(0,0,1), Point(0,0,1), Point(0,0,1));
    EXPECT_FALSE(t1.intersect(t2));
    EXPECT_FALSE(t2.intersect(t1));
}

TEST(intersect_3d, Degenerate_Point_Segment_Intersection) {
    Triangle t1(Point(0,0,2), Point(0,0,3), Point(0,0,-1));
    Triangle t2(Point(0,0,1), Point(0,0,1), Point(0,0,1));
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, Degenerate_Two_Points_NoIntersection) {
    Triangle t1(Point(2,0,0), Point(2,0,0), Point(2,0,0));
    Triangle t2(Point(0,0,1), Point(0,0,1), Point(0,0,1));
    EXPECT_FALSE(t1.intersect(t2));
    EXPECT_FALSE(t2.intersect(t1));
}

TEST(intersect_3d, Degenerate_Two_Points_Intersection) {
    Triangle t1(Point(2,0,0), Point(2,0,0), Point(2,0,0));
    Triangle t2(Point(2,0,0), Point(2,0,0), Point(2,0,0));
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, Coplanar_Disjoint_NoIntersection) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(3,0,0), Point(5,0,0), Point(3,2,0));
    EXPECT_FALSE(t1.intersect(t2));
    EXPECT_FALSE(t2.intersect(t1));
}

TEST(intersect_3d, Coplanar_ShareFullEdge_Touching) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(2,0,0), Point(0,2,0), Point(2,2,0)); // общее ребро
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, Coplanar_Containment_T2InsideT1) {
    Triangle t1(Point(0,0,0), Point(3,0,0), Point(0,3,0));
    Triangle t2(Point(0.5,0.5,0), Point(1.5,0.5,0), Point(0.5,1.5,0));
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, Coplanar_TouchAtSingleVertexOnly) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(2,0,0), Point(3,0,0), Point(2,1,0)); // касание в вершине (2,0,0)
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, Coplanar_IdenticalTriangles) {
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, SkewPlanes_IntersectInSegment) {
    // плоскости не параллельны; линия пересечения даёт отрезок внутри обоих
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0.2,0.2,-1), Point(1.5,0.2,1), Point(0.2,1.5,1));
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, SkewPlanes_LineCutsOutside_NoIntersection) {
    // t2 пересекает плоскость t1, но сечение вне t1
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(3,3,-1), Point(3,3,1), Point(4,2,0.5));
    EXPECT_FALSE(t1.intersect(t2));
    EXPECT_FALSE(t2.intersect(t1));
}

TEST(intersect_3d, SkewPlanes_InteriorSegment_Diagonal) {
    // Две непараллельные плоскости, пересечение — отрезок внутри обоих треугольников
    Triangle t1(Point(0,0,0), Point(3,0,0), Point(0,3,0));
    Triangle t2(Point(0.3,0.3,-1), Point(2.4,0.3,1), Point(0.3,2.4,1));
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, SkewPlanes_InteriorSegment_TouchesT1Edges) {
    // Отрезок пересечения лежит от (1,0,0) до (0,1,0) — касается рёбер t1
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0,0,-1), Point(2,0,1), Point(0,2,1));
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, EdgeFace_Pierce_InteriorPoint) {
    // Ребро t2 протыкает внутренность t1 в одной точке
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0.8,0.6,-1), Point(0.8,0.6,1), Point(3,-1,0.5));
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, VertexOnT1Edge_PointContact) {
    // Вершина t2 лежит на ребре t1, остальные точки вне плоскости t1
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(1,0,0), Point(1,0,1), Point(2,1,1));
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, NonCoplanar_ShareFullEdge) {
    // Общий полный ребро при непараллельных плоскостях (линия пересечения = ребро)
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(0,0,0), Point(2,0,0), Point(1,0,1)); // плоскость y=0
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, SkewPlanes_LineHitsOutside_NoIntersection_2) {
    // Линия пересечения плоскостей проходит вне t1
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(0,2,0));
    Triangle t2(Point(3,3,-1), Point(5,3,1), Point(3,5,1));
    EXPECT_FALSE(t1.intersect(t2));
    EXPECT_FALSE(t2.intersect(t1));
}

TEST(intersect_3d, Coplanar_PartialAreaOverlap) {
    // Копланарные треугольники с непустой площадью пересечения
    Triangle t1(Point(0,0,0), Point(3,0,0), Point(0,3,0));
    Triangle t2(Point(0.5,0,0), Point(2.5,0,0), Point(0.5,2,0));
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
}

TEST(intersect_3d, Coplanar_ProperEdgeCross) {
    // Копланарные: рёбра пересекаются, ни одна вершина не лежит внутри другого
    Triangle t1(Point(0,0,0), Point(2,0,0), Point(1,2,0));
    Triangle t2(Point(-0.5,0.5,0), Point(2.5,0.5,0), Point(1.5,-1,0));
    EXPECT_TRUE(t1.intersect(t2));
    EXPECT_TRUE(t2.intersect(t1));
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
    EXPECT_FALSE(A.intersect_2d(B));
    EXPECT_FALSE(B.intersect_2d(A));
}

TEST(Intersect2D, Containment_B_inside_A) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(0.2,0.2), P(0.6,0.2), P(0.2,0.6) };

    // act, assert
    EXPECT_TRUE(A.intersect_2d(B));
    EXPECT_TRUE(B.intersect_2d(A));
}

TEST(Intersect2D, Containment_OrientationDoesNotMatter) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(0.2,0.6), P(0.6,0.2), P(0.2,0.2) };

    // act, assert
    EXPECT_TRUE(A.intersect_2d(B));
    EXPECT_TRUE(B.intersect_2d(A));
}

TEST(Intersect2D, EdgeEdge_ProperCross_NoVertexInside) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(1,2) };
    Triangle B{ P(-0.5,0.5), P(2.5,0.5), P(1.5,-1) };

    // act, assert
    EXPECT_TRUE(A.intersect_2d(B));
    EXPECT_TRUE(B.intersect_2d(A));
}

TEST(Intersect2D, Touch_VertexOnEdge_PointContact) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(1,0), P(1.1,-0.2), P(0.9,-0.2) };

    // act, assert
    EXPECT_TRUE(A.intersect_2d(B));
    EXPECT_TRUE(B.intersect_2d(A));
}

TEST(Intersect2D, Collinear_OverlappingEdges) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(0,0), P(2,0), P(3,1) };

    // act, assert
    EXPECT_TRUE(A.intersect_2d(B));
    EXPECT_TRUE(B.intersect_2d(A));
}

TEST(Intersect2D, Touch_AtSingleSharedVertexOnly) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(2,0), P(3,0), P(2,1) };

    // act, assert
    EXPECT_TRUE(A.intersect_2d(B));
    EXPECT_TRUE(B.intersect_2d(A));
}

TEST(Intersect2D, Disjoint_CloseButSeparated) {
    // arrange
    Triangle A{ P(0,0), P(2,0), P(0,2) };
    Triangle B{ P(2.1,0.0), P(3.0,0.0), P(2.1,0.9) };

    // act, assert
    EXPECT_FALSE(A.intersect_2d(B));
    EXPECT_FALSE(B.intersect_2d(A));
}
