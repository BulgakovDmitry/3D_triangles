#ifndef INCLUDE_TRIANGLE_TO_TRIANGLE_HPP
#define INCLUDE_TRIANGLE_TO_TRIANGLE_HPP

#include <array>

#include "common/cmp.hpp"
#include "intersection/point_to_triangle.hpp"
#include "intersection/segment_to_triangle.hpp"
#include "intersection/triangle_to_triangle_2d.hpp"
#include "primitives/point.hpp"
#include "primitives/triangle.hpp"
#include "primitives/vector.hpp"

namespace intersection_3d {

enum class Sign {
    different,
    pozitive,
    negative,
    common_vertice_other_poz_or_neg,
    common_plane,
};

template <std::floating_point T>
inline T orient_3d(const Point<T> &p_1, const Point<T> &q_1, const Point<T> &r_1,
                   const Point<T> &p_2) {
    Vector<T> p_q(q_1.get_x() - p_1.get_x(), q_1.get_y() - p_1.get_y(), q_1.get_z() - p_1.get_z());
    Vector<T> p_r(r_1.get_x() - p_1.get_x(), r_1.get_y() - p_1.get_y(), r_1.get_z() - p_1.get_z());
    Vector<T> p_p(p_2.get_x() - p_1.get_x(), p_2.get_y() - p_1.get_y(), p_2.get_z() - p_1.get_z());

    return mixed_product(p_q, p_r, p_p);
}

template <std::floating_point T>
inline void update_sign_orient(const Triangle<T> &base, const Triangle<T> &ref,
                               std::array<T, 3> &signs) {
    auto vertices_base = base.get_vertices();
    auto vertices_ref = ref.get_vertices();

    signs[0] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_base[0]);
    signs[1] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_base[1]);
    signs[2] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_base[2]);
}

template <std::floating_point T>
inline bool check_common_vertice(T sign_plane_p, T sign_plane_r, T sign_plane_q) {
    bool vert_p_in_plane = cmp::is_zero(sign_plane_p) &&
                           ((cmp::pozitive(sign_plane_r) && cmp::pozitive(sign_plane_q)) ||
                            (cmp::negative(sign_plane_r) && cmp::negative(sign_plane_q)));

    bool vert_r_in_plane = cmp::is_zero(sign_plane_r) &&
                           ((cmp::pozitive(sign_plane_p) && cmp::pozitive(sign_plane_q)) ||
                            (cmp::negative(sign_plane_p) && cmp::negative(sign_plane_q)));

    bool vert_q_in_plane = cmp::is_zero(sign_plane_q) &&
                           ((cmp::pozitive(sign_plane_r) && cmp::pozitive(sign_plane_p)) ||
                            (cmp::negative(sign_plane_r) && cmp::negative(sign_plane_p)));

    return (vert_p_in_plane || vert_r_in_plane || vert_q_in_plane);
}

template <std::floating_point T>
inline size_t get_common_vertice(T sign_plane_p, T sign_plane_r, T sign_plane_q) {
    if (cmp::is_zero(sign_plane_p) &&
        ((cmp::pozitive(sign_plane_r) && cmp::pozitive(sign_plane_q)) ||
         (cmp::negative(sign_plane_r) && cmp::negative(sign_plane_q))))
        return 0;

    if (cmp::is_zero(sign_plane_q) &&
        ((cmp::pozitive(sign_plane_r) && cmp::pozitive(sign_plane_p)) ||
         (cmp::negative(sign_plane_r) && cmp::negative(sign_plane_p))))
        return 1;

    if (cmp::is_zero(sign_plane_r) &&
        ((cmp::pozitive(sign_plane_p) && cmp::pozitive(sign_plane_q)) ||
         (cmp::negative(sign_plane_p) && cmp::negative(sign_plane_q))))
        return 2;

    return 0;
}

template <std::floating_point T>
inline Sign check_relative_positions(const Triangle<T> &first, const Triangle<T> &second) {
    std::array<T, 3> signs;

    update_sign_orient(first, second, signs);

    if (cmp::pozitive(signs[0]) && cmp::pozitive(signs[1]) && cmp::pozitive(signs[2]))
        return Sign::pozitive;

    if (cmp::negative(signs[0]) && cmp::negative(signs[1]) && cmp::negative(signs[2]))

        return Sign::negative;

    if (cmp::is_zero(signs[0]) && cmp::is_zero(signs[1]) && cmp::is_zero(signs[2]))
        return Sign::common_plane;

    if (check_common_vertice(signs[0], signs[1], signs[2]))
        return Sign::common_vertice_other_poz_or_neg;

    update_sign_orient(second, first, signs);

    if (cmp::pozitive(signs[0]) && cmp::pozitive(signs[1]) && cmp::pozitive(signs[2]))
        return Sign::pozitive;

    if (cmp::negative(signs[0]) && cmp::negative(signs[1]) && cmp::negative(signs[2]))
        return Sign::negative;

    if (cmp::is_zero(signs[0]) && cmp::is_zero(signs[1]) && cmp::is_zero(signs[2]))
        return Sign::common_plane;

    if (check_common_vertice(signs[0], signs[1], signs[2]))
        return Sign::common_vertice_other_poz_or_neg;

    return Sign::different;
}

template <std::floating_point T>
inline bool check_segments_intersect(const Triangle<T> &canon_main, const Triangle<T> &canon_ref) {
    auto vertices_main = canon_main.get_vertices();
    auto vertices_ref = canon_ref.get_vertices();

    auto sign_1 = orient_3d(vertices_main[0], vertices_main[1], vertices_ref[0], vertices_ref[1]);
    auto sign_2 = orient_3d(vertices_main[0], vertices_main[2], vertices_ref[2], vertices_ref[0]);

    if (cmp::non_negative(sign_1) && cmp::non_negative(sign_2))
        return true;

    if (cmp::non_pozitive(sign_1) && cmp::non_pozitive(sign_2))
        return true;

    return false;
}

template <std::floating_point T>
inline Triangle<T> canonicalize_triangle(const Triangle<T> &base, const Triangle<T> &ref) {
    std::array<T, 3> signs;
    auto canon = base;

    update_sign_orient(canon, ref, signs);

    if (cmp::negative(signs[0]) && cmp::non_negative(signs[1]) && cmp::non_negative(signs[2]))
        return canon;

    if (cmp::non_negative(signs[0]) && cmp::non_negative(signs[1]) && cmp::negative(signs[2])) {
        canon.rotate_clockwise();
        return canon;
    }

    if (cmp::non_negative(signs[0]) && cmp::negative(signs[1]) && cmp::non_negative(signs[2])) {
        canon.rotate_clockwise();
        canon.rotate_clockwise();
        return canon;
    }

    if (cmp::non_pozitive(signs[0]) && cmp::non_pozitive(signs[1]) && cmp::pozitive(signs[2]))
        canon.rotate_clockwise();

    else if (cmp::non_pozitive(signs[0]) && cmp::pozitive(signs[1]) &&
             cmp::non_pozitive(signs[2])) {
        canon.rotate_clockwise();
        canon.rotate_clockwise();
    }

    update_sign_orient(canon, ref, signs);

    if (cmp::pozitive(signs[0]))
        canon.swap_vertices(1, 2);

    return canon;
}

template <std::floating_point T>
inline bool intersect_one_vertice_in_plane(const Triangle<T> &first, const Triangle<T> &second) {
    size_t common_vertex;
    std::array<T, 3> signs;

    update_sign_orient(first, second, signs);

    if (check_common_vertice(signs[0], signs[1], signs[2])) {
        common_vertex = get_common_vertice(signs[0], signs[1], signs[2]);
        if (point_inside_triangle(second, first.get_vertices()[common_vertex]))
            return true;
        return false;
    }

    update_sign_orient(second, first, signs);

    if (check_common_vertice(signs[0], signs[1], signs[2])) {
        common_vertex = get_common_vertice(signs[0], signs[1], signs[2]);
        if (point_inside_triangle(first, second.get_vertices()[common_vertex]))
            return true;
    }

    return false;
}

template <std::floating_point T>
inline bool intersect(const Triangle<T> &first, const Triangle<T> &second) {
    if (first.get_type() == TypeTriangle::point)
        return point_inside_triangle(second, first.get_vertices()[0]);
    if (second.get_type() == TypeTriangle::point)
        return point_inside_triangle(first, second.get_vertices()[0]);
    if (first.get_type() == TypeTriangle::interval)
        return segment_intersect_triangle(/*triangle=*/second, /*interval=*/first);
    if (second.get_type() == TypeTriangle::interval)
        return segment_intersect_triangle(/*triangle=*/first, /*interval=*/second);

    Sign relative_positions = check_relative_positions(first, second);

    if (relative_positions == Sign::pozitive || relative_positions == Sign::negative)
        return false;

    if (relative_positions == Sign::common_plane)
        return intersection_2d::intersect_2d(first, second); // 2d case

    if (relative_positions == Sign::common_vertice_other_poz_or_neg)
        return intersect_one_vertice_in_plane(first, second);

    auto canon_main = canonicalize_triangle(first, second);
    auto canon_ref = canonicalize_triangle(second, first);

    return check_segments_intersect(canon_main, canon_ref);
}

} // namespace intersection_3d

#endif
