#ifndef SEGMENT_TO_TRIANGLE_HPP
#define SEGMENT_TO_TRIANGLE_HPP

#include "common/cmp.hpp"
#include "intersection/point_to_triangle.hpp"
#include "intersection/segment_to_segment.hpp"
#include "primitives/point.hpp"
#include "primitives/triangle.hpp"
#include "primitives/vector.hpp"

inline bool check_segment_triangle_intersection_2d(const Point &seg_start, const Point &seg_end,
                                                   const Triangle &triangle) {
    auto vertices = triangle.get_vertices();

    // Check if the ends of the segment lie inside the triangle
    if (point_inside_triangle(triangle, seg_start) || point_inside_triangle(triangle, seg_end))
        return true;

    // Check the intersection of the segment with each edge of the triangle
    for (int i = 0; i < 3; ++i) {
        int j = (i + 1) % 3;
        if (check_segments_intersect_2d(seg_start, seg_end, vertices[i], vertices[j]))
            return true;
    }

    return false;
}

inline bool segment_intersect_triangle(const Triangle &triangle, const Triangle &interval) {
    if (triangle.get_type() == TypeTriangle::point) {
        auto vertices = interval.get_vertices();
        auto ends = interval.get_interval();
        return is_point_on_segment(vertices[ends.first], vertices[ends.second],
                                   triangle.get_vertices()[0]);
    }

    if (triangle.get_type() == TypeTriangle::interval) {
        auto vertices_2 = triangle.get_vertices();
        auto ends_2 = triangle.get_interval();

        auto vertices_1 = interval.get_vertices();
        auto ends_1 = interval.get_interval();

        return check_segments_intersect_3d(vertices_1[ends_1.first], vertices_1[ends_1.second],
                                           vertices_2[ends_2.first], vertices_2[ends_2.second]);
    }

    std::pair<size_t, size_t> interval_ends = interval.get_interval();

    auto interval_vertices = interval.get_vertices();
    auto int_start = interval_vertices[interval_ends.first];
    auto int_end = interval_vertices[interval_ends.second];

    auto vertices = triangle.get_vertices();

    Vector edge_1{vertices[0], vertices[1]};
    Vector edge_2{vertices[0], vertices[2]};
    Vector normal = vector_product(edge_1, edge_2);

    // direct interval
    Vector int_dir{int_start, int_end};

    // check parallel
    float denom = scalar_product(normal, int_dir);
    if (cmp::is_zero(denom)) {
        // check whether the segment lies in the plane of the triangle
        if (is_in_plane(int_start, triangle) && is_in_plane(int_end, triangle))
            return check_segment_triangle_intersection_2d(int_start, int_end, triangle);

        return false; // interval is parallel of plane triangle
    }

    // calculate parameter t of intersect with plane
    Vector to_triangle{int_start, vertices[0]};
    float t = scalar_product(normal, to_triangle) / denom;

    // chech that intersection with plane is in interval
    if (cmp::negative(t) || cmp::greater(t, 1.0f))
        return false;

    // calculate point of intersection
    Point point{int_start.get_x() + int_dir.get_x() * t, int_start.get_y() + int_dir.get_y() * t,
                int_start.get_z() + int_dir.get_z() * t};

    // check that point of intersection in triangle
    if (point_inside_triangle(triangle, point))
        return true;

    return false;
}

#endif
