#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <array>
#include <utility>

#include "primitives/point.hpp"
#include "primitives/vector.hpp"

const double eps = 1e-12;

enum sign_t {
    different = 2,
    pozitive  = 1,
    null_sign = 0,
    negative  = -1,
};

double mixed_product(const Vector &a, const Vector &b, const Vector &c) {
    return scalar_product(vector_product(a, b), c);
}

double orient_3d(const Point &p_1, const Point &q_1, const Point &r_1, const Point &p_2) {
    Vector p_q(q_1.get_x() - p_1.get_x(), q_1.get_y() - p_1.get_y(), q_1.get_z() - p_1.get_z());
    Vector p_r(r_1.get_x() - p_1.get_x(), r_1.get_y() - p_1.get_y(), r_1.get_z() - p_1.get_z());
    Vector p_p(p_2.get_x() - p_1.get_x(), p_2.get_y() - p_1.get_y(), p_2.get_z() - p_1.get_z());

    return mixed_product(p_q, p_r, p_p);
}

class Triangle;

Triangle canonicalize_triangle(const Triangle &base, const Triangle &ref);

class Triangle {
    Point vertices_[3];

public:
    Triangle(const Point &point_0, const Point &point_1, const Point &point_2)
        : vertices_{point_0, point_1, point_2} {}

    const Point (&get_vertices() const)[3] { return vertices_; }

    bool intersect(const Triangle &triangle) const {
        // check the position of the vertices of one triangle relative to another
        auto relative_positions = check_relative_positions(triangle);

        if (relative_positions == pozitive || relative_positions == negative)
            return false;

        if (relative_positions == null_sign)
            return intersect_2d(triangle); // 2d case

        auto canon_main = canonicalize_triangle(*this, triangle);
        auto canon_ref  = canonicalize_triangle(triangle, *this);

        return check_interval_intersect(canon_main, canon_ref);
    }

    /// TODO: 2d intersect
    bool intersect_2d(const Triangle &triangle) const { return true; }

    void rotate_vertices() {
        auto copy    = vertices_[0];
        vertices_[0] = vertices_[1];
        vertices_[1] = vertices_[2];
        vertices_[2] = copy;
    }

    void swap_vertices(int i, int j) { std::swap(vertices_[i], vertices_[j]); }

private:
    bool check_interval_intersect(const Triangle &canon_main, const Triangle &canon_ref) const {
        auto vertices_main = canon_main.get_vertices();
        auto vertices_ref  = canon_ref.get_vertices();

        auto sign_1 =
            orient_3d(vertices_main[0], vertices_main[1], vertices_ref[0], vertices_ref[1]);
        auto sign_2 =
            orient_3d(vertices_main[0], vertices_main[2], vertices_ref[2], vertices_ref[0]);

        if (sign_1 > eps && sign_2 > eps)
            return true;

        return false;
    }

    sign_t check_relative_positions(const Triangle &triangle) const {
        auto vertices_2     = triangle.get_vertices();

        auto sign_plane2_p1 = orient_3d(vertices_2[0], vertices_2[1], vertices_2[2], vertices_[0]);
        auto sign_plane2_q1 = orient_3d(vertices_2[0], vertices_2[1], vertices_2[2], vertices_[1]);
        auto sign_plane2_r1 = orient_3d(vertices_2[0], vertices_2[1], vertices_2[2], vertices_[2]);

        if ((sign_plane2_p1 > eps && sign_plane2_r1 > eps && sign_plane2_q1 > eps))
            return pozitive;

        if (sign_plane2_p1 < -eps && sign_plane2_r1 < -eps && sign_plane2_q1 < -eps)
            return negative;

        auto sign_plane1_p2 = orient_3d(vertices_[0], vertices_[1], vertices_[2], vertices_2[0]);
        auto sign_plane1_q2 = orient_3d(vertices_[0], vertices_[1], vertices_[2], vertices_2[1]);
        auto sign_plane1_r2 = orient_3d(vertices_[0], vertices_[1], vertices_[2], vertices_2[2]);

        if (sign_plane1_p2 > eps && sign_plane1_r2 > eps && sign_plane1_q2 > eps)
            return pozitive;

        if (sign_plane1_p2 < -eps && sign_plane1_r2 < -eps && sign_plane1_q2 < -eps)
            return negative;

        return different;
    }
};

Triangle canonicalize_triangle(const Triangle &base, const Triangle &ref) {
    std::array<double, 3> signs;
    auto                  canon         = base;
    auto                  vertices_base = base.get_vertices();
    auto                  vertices_ref  = ref.get_vertices();

    signs[0] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_base[0]);
    signs[1] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_base[1]);
    signs[2] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_base[2]);

    if (signs[0] > eps && signs[1] < -eps && signs[2] < -eps)
        return canon;

    else if (signs[0] < -eps && signs[1] > eps && signs[2] < -eps)
        canon.rotate_vertices();

    else if (signs[0] < -eps && signs[1] < -eps && signs[2] > eps) {
        canon.rotate_vertices();
        canon.rotate_vertices();
    }

    signs[0] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_base[0]);
    signs[1] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_base[1]);
    signs[2] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_base[2]);

    if (signs[0] < -eps)
        canon.swap_vertices(1, 2);

    return canon;
}

#endif
