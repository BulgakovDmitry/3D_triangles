#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <array>

#include "primitives/interval.hpp"
#include "primitives/line.hpp"
#include "primitives/point.hpp"
#include "primitives/polygon.hpp"
#include "primitives/vector.hpp"

enum sign_t {
    different = 2,
    pozitive  = 1,
    null_sign = 0,
    negative  = -1,
};

const double eps = 1e-12;

double       mixed_product(const Vector &a, const Vector &b, const Vector &c) {
    return scalar_product(vector_product(a, b), c);
}

double orient_3d(const Point &p_1, const Point &q_1, const Point &r_1, const Point &p_2) {
    Vector p_q(q_1.get_x() - p_1.get_x(), q_1.get_y() - p_1.get_y(), q_1.get_z() - p_1.get_z());
    Vector p_r(r_1.get_x() - p_1.get_x(), r_1.get_y() - p_1.get_y(), r_1.get_z() - p_1.get_z());
    Vector p_p(p_2.get_x() - p_1.get_x(), p_2.get_y() - p_1.get_y(), p_2.get_z() - p_1.get_z());

    return mixed_product(p_q, p_r, p_p);
}

class Triangle {
    Point   vertices_[3];
    Polygon polygon_;

public:
    Triangle(const Point &point_0, const Point &point_1, const Point &point_2)
        : vertices_{point_0, point_1, point_2} {}

    const Polygon &get_polygon() const { return polygon_; }

    const Point (&get_vertices() const)[3] { return vertices_; }

    bool intersect(const Triangle &triangle) const {
        // check the position of the vertices of one triangle relative to another
        auto relative_positions = check_relative_positions(triangle);

        if (relative_positions == pozitive || relative_positions == negative)
            return false;

        if (relative_positions == null_sign)
            return intersect_2d(triangle); // coplanar case

        canonicalize_triangle(triangle);
        triangle.canonicalize_triangle();

        return true;
    }

    bool intersect_2d(const Triangle &triangle) const { return true; }

    void canonicalize_triangle(const Triangle &ref_triangle) {
        std::array<double, 3> signs;
        auto                  vertices_ref = ref_triangle.get_vertices();

        signs[0] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_[0]);
        signs[1] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_[1]);
        signs[2] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_[2]);

        if (signs[0] == pozitive && signs[1] == negative && signs[2] == negative)
            return;

        else if (signs[0] == negative && signs[1] == pozitive && signs[2] == negative)
            rotate_vertices();

        else if (signs[0] == negative && signs[1] == negative && signs[2] == pozitive) {
            rotate_vertices();
            rotate_vertices();
        }

        signs[0] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_[0]);
        signs[1] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_[1]);
        signs[2] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_[2]);

        if (signs[0] == negative)
            std::swap(vertices_[1], vertices_[2]);
    }

private:
    void rotate_vertices() {
        auto copy    = vertices_[0];
        vertices_[0] = vertices_[1];
        vertices_[1] = vertices_[2];
        vertices_[2] = copy;
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

#endif
