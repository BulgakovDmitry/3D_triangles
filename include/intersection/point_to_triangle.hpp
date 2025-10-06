#ifndef POINT_TO_TRIANGLE_HPP
#define POINT_TO_TRIANGLE_HPP

#include "common/cmp.hpp"
#include "intersection/point_to_segment.hpp"
#include "primitives/point.hpp"
#include "primitives/triangle.hpp"

using triangle::Triangle;
using triangle::TypeTriangle;

inline bool is_in_plane(const Point &point, const Triangle &triangle) {
    auto vertices = triangle.get_vertices();

    Vector normal =
        vector_product(Vector{vertices[1], vertices[0]}, Vector{vertices[2], vertices[0]});
    Vector to_point{point, vertices[0]};
    return cmp::is_zero(scalar_product(normal, to_point));
}

inline bool point_inside_triangle(const Triangle &triangle, const Point &point) {
    if (triangle.get_type() == TypeTriangle::point)
        return triangle.get_vertices()[0] == point;

    if (triangle.get_type() == TypeTriangle::interval) {
        auto vertices = triangle.get_vertices();
        auto segment = triangle.get_interval();
        return is_point_on_segment(vertices[segment.first], vertices[segment.second], point);
    }

    if (!is_in_plane(point, triangle))
        return false;

    auto vertices = triangle.get_vertices();

    // Calculate vectors from the vertices of the triangle to the point
    Vector v0(vertices[0], vertices[1]);
    Vector v1(vertices[0], vertices[2]);
    Vector v2(vertices[0], point);

    // Calculate dot-products
    double dot00 = scalar_product(v0, v0);
    double dot01 = scalar_product(v0, v1);
    double dot02 = scalar_product(v0, v2);
    double dot11 = scalar_product(v1, v1);
    double dot12 = scalar_product(v1, v2);

    // Calculate barycentric coordinates
    double invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if the point lies inside the triangle
    return (u >= -cmp::float_eps) && (v >= -cmp::float_eps) && (u + v <= 1.0 + cmp::float_eps);
}

#endif
