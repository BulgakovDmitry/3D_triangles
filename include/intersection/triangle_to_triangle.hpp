#ifndef INCLUDE_TRIANGLE_TO_TRIANGLE_HPP
#define INCLUDE_TRIANGLE_TO_TRIANGLE_HPP

#include "../primitives/triangle.hpp"

namespace triangle {

enum class Sign {
    different,
    pozitive,
    negative,
    common_vertice_other_poz_or_neg,
    common_plane,
};

bool intersect(const Triangle &first, const Triangle &second) const {
    if (first.get_type() == TypeTriangle::point)
        return point_inside_triangle(triangle, vertices_[0]);
    if (second.get_type() == TypeTriangle::point)
        return point_inside_triangle(*this, triangle.get_vertices()[0]);
    if (type_ == TypeTriangle::interval)
        return interval_intersect_triangle(/*triangle=*/triangle, /*interval=*/*this);
    if (triangle.get_type() == TypeTriangle::interval)
        return interval_intersect_triangle(/*triangle=*/*this, /*interval=*/triangle);

    Sign relative_positions = check_relative_positions(triangle);

    if (relative_positions == Sign::pozitive || relative_positions == Sign::negative)
        return false;

    if (relative_positions == Sign::common_plane)

        return intersect_2d(triangle); // 2d case

    if (relative_positions == Sign::common_vertice_other_poz_or_neg)
        return intersect_one_vertice_in_plane(triangle);

    auto canon_main = canonicalize_triangle(*this, triangle);
    auto canon_ref = canonicalize_triangle(triangle, *this);

    return check_interval_intersect(canon_main, canon_ref);
}

} // namespace triangle
