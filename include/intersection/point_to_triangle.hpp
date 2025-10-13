#ifndef POINT_TO_TRIANGLE_HPP
#define POINT_TO_TRIANGLE_HPP

#include "common/cmp.hpp"
#include "intersection/point_to_segment.hpp"
#include "primitives/point.hpp"
#include "primitives/triangle.hpp"

using triangle::Triangle;
using triangle::TypeTriangle;

template <std::floating_point T>
bool is_in_plane(const triangle::Point<T> &point, const Triangle<T> &triangle) {
    const auto &vertices = triangle.get_vertices();

    return cmp::is_zero(mixed_product(triangle::Vector<T>{vertices[1], vertices[0]},
                                      triangle::Vector<T>{vertices[2], vertices[0]},
                                      triangle::Vector<T>{point, vertices[0]}));
}

template <std::floating_point T>
bool point_inside_triangle(const Triangle<T> &triangle, const triangle::Point<T> &point) {
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
    triangle::Vector<T> v0(vertices[0], vertices[1]);
    triangle::Vector<T> v1(vertices[0], vertices[2]);
    triangle::Vector<T> v2(vertices[0], point);

    // Calculate dot-products
    T dot00 = scalar_product(v0, v0);
    T dot01 = scalar_product(v0, v1);
    T dot02 = scalar_product(v0, v2);
    T dot11 = scalar_product(v1, v1);
    T dot12 = scalar_product(v1, v2);

    // Calculate barycentric coordinates
    T invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
    T u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    T v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if the point lies inside the triangle
    return (u >= -cmp::precision<T>::epsilon) && (v >= -cmp::precision<T>::epsilon) && (u + v <= 1.0 + cmp::precision<T>::epsilon);
}

#endif
