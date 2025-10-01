#ifndef INCLUDE_TRIANGLE_HPP
#define INCLUDE_TRIANGLE_HPP

#include <array>
#include <iostream>
#include <utility>

#include "../common/cmp.hpp"
#include "primitives/point.hpp"
#include "primitives/vector.hpp"

namespace triangle {

enum class Sign {
    different,
    pozitive,
    negative,
    common_vertice_other_poz_or_neg,
    common_plane,
};

bool check_common_vertice(float sign_plane_p, float sign_plane_r, float sign_plane_q) {
    bool vert_p_in_plane = cmp::is_zero(sign_plane_p) &&
                               (cmp::pozitive(sign_plane_r) && cmp::pozitive(sign_plane_q)) ||
                           (cmp::negative(sign_plane_r) && cmp::negative(sign_plane_q));

    bool vert_r_in_plane = cmp::is_zero(sign_plane_r) &&
                               (cmp::pozitive(sign_plane_p) && cmp::pozitive(sign_plane_q)) ||
                           (cmp::negative(sign_plane_p) && cmp::negative(sign_plane_q));

    bool vert_q_in_plane = cmp::is_zero(sign_plane_q) &&
                               (cmp::pozitive(sign_plane_r) && cmp::pozitive(sign_plane_p)) ||
                           (cmp::negative(sign_plane_r) && cmp::negative(sign_plane_p));

    return (vert_p_in_plane || vert_r_in_plane || vert_q_in_plane);
}

size_t get_common_vertice(float sign_plane_p, float sign_plane_r, float sign_plane_q) {
    if (cmp::is_zero(sign_plane_p) &&
            (cmp::pozitive(sign_plane_r) && cmp::pozitive(sign_plane_q)) ||
        (cmp::negative(sign_plane_r) && cmp::negative(sign_plane_q)))
        return 0;

    if (cmp::is_zero(sign_plane_q) &&
            (cmp::pozitive(sign_plane_r) && cmp::pozitive(sign_plane_p)) ||
        (cmp::negative(sign_plane_r) && cmp::negative(sign_plane_p)))
        return 1;

    if (cmp::is_zero(sign_plane_r) &&
            (cmp::pozitive(sign_plane_p) && cmp::pozitive(sign_plane_q)) ||
        (cmp::negative(sign_plane_p) && cmp::negative(sign_plane_q)))
        return 2;

    exit(EXIT_FAILURE);
}

inline double orient_3d(const Point &p_1, const Point &q_1, const Point &r_1, const Point &p_2) {
    Vector p_q(q_1.get_x() - p_1.get_x(), q_1.get_y() - p_1.get_y(), q_1.get_z() - p_1.get_z());
    Vector p_r(r_1.get_x() - p_1.get_x(), r_1.get_y() - p_1.get_y(), r_1.get_z() - p_1.get_z());
    Vector p_p(p_2.get_x() - p_1.get_x(), p_2.get_y() - p_1.get_y(), p_2.get_z() - p_1.get_z());

    return mixed_product(p_q, p_r, p_p);
}

inline double orient_2d(const Point &a, const Point &b, const Point &c, const Vector &n) {
    return mixed_product(Vector(a, b), Vector(a, c), n);
}

class Triangle;
Triangle canonicalize_triangle(const Triangle &base, const Triangle &ref);

class Triangle {
private:
    Point vertices_[3];

public:
    Triangle(const Point &point_0, const Point &point_1, const Point &point_2)
        : vertices_{point_0, point_1, point_2} {}

    const Point (&get_vertices() const)[3] { return vertices_; }

    bool intersect(const Triangle &triangle) const {
        // check the position of the vertices of one triangle relative to another
        Sign relative_positions = check_relative_positions(triangle);

        if (relative_positions == Sign::pozitive || relative_positions == Sign::negative)
            return false;

        if (relative_positions == Sign::common_plane)
            return intersect_2d(triangle); // 2d case

        if (relative_positions == Sign::common_vertice_other_poz_or_neg)
            return intersect_one_vertice_in_plane(triangle);

        auto canon_main = canonicalize_triangle(*this, triangle);
        auto canon_ref  = canonicalize_triangle(triangle, *this);

        return check_interval_intersect(canon_main, canon_ref);
    }

    bool intersect_2d(const Triangle &other) const {
        const auto &A = get_vertices();
        const auto &B = other.get_vertices();

        Vector      n = vector_product(Vector(A[0], A[1]), Vector(A[0], A[2]));

        for (int i = 0; i < 3; ++i) {
            auto relative_positions_2d = check_relative_positions_2d(A[i], B[0], B[1], B[2], n);
            if (relative_positions_2d == Sign::pozitive || relative_positions_2d == Sign::negative)
                return true;
        }

        for (int j = 0; j < 3; ++j) {
            auto relative_positions_2d = check_relative_positions_2d(B[j], A[0], A[1], A[2], n);
            if (relative_positions_2d == Sign::pozitive || relative_positions_2d == Sign::negative)
                return true;
        }

        for (int i = 0; i < 3; ++i) {
            int inext = (i + 1) % 3;
            for (int j = 0; j < 3; ++j) {
                int jnext = (j + 1) % 3;
                if (check_interval_intersect_2d(A[i], A[inext], B[j], B[jnext], n))
                    return true;
            }
        }

        return false;
    }

    void rotate_vertices() {
        auto copy    = vertices_[0];
        vertices_[0] = vertices_[1];
        vertices_[1] = vertices_[2];
        vertices_[2] = copy;
    }
    void swap_vertices(int i, int j) { std::swap(vertices_[i], vertices_[j]); }

    void print() const {
        std::cout << BLUE << "triangle " << CEAN << "{\n" << RESET;
        for (int i = 0; i < 3; ++i) {
            std::cout << "   ";
            vertices_[i].print();
        }
        std::cout << CEAN << "}" << RESET << std::endl;
    }

private:
    bool intersect_one_vertice_in_plane(const Triangle &ref) {
        auto common_vertice = get_common_vertice()
    }

    bool check_interval_intersect(const Triangle &canon_main, const Triangle &canon_ref) const {
        auto vertices_main = canon_main.get_vertices();
        auto vertices_ref  = canon_ref.get_vertices();

        canon_main.print();
        canon_ref.print();

        auto sign_1 =
            orient_3d(vertices_main[0], vertices_main[1], vertices_ref[0], vertices_ref[1]);
        auto sign_2 =
            orient_3d(vertices_main[0], vertices_main[2], vertices_ref[2], vertices_ref[0]);

        std::cout << "sign_1: " << sign_1 << " sign_2: " << sign_2 << '\n';

        if (sign_1 > float_eps && sign_2 > float_eps)
            return true;

        return false;
    }

    Sign check_relative_positions(const Triangle &triangle) const {
        auto vertices_2     = triangle.get_vertices();

        auto sign_plane2_p1 = orient_3d(vertices_2[0], vertices_2[1], vertices_2[2], vertices_[0]);
        auto sign_plane2_q1 = orient_3d(vertices_2[0], vertices_2[1], vertices_2[2], vertices_[1]);
        auto sign_plane2_r1 = orient_3d(vertices_2[0], vertices_2[1], vertices_2[2], vertices_[2]);

        if (cmp::pozitive(sign_plane2_p1) && cmp::pozitive(sign_plane2_r1) &&
            cmp::pozitive(sign_plane2_q1))
            return Sign::pozitive;

        if (cmp::negative(sign_plane2_p1) && cmp::negative(sign_plane2_r1) &&
            cmp::negative(sign_plane2_q1))
            return Sign::negative;

        if (cmp::is_zero(sign_plane2_p1), cmp::is_zero(sign_plane2_r1),
            cmp::is_zero(sign_plane2_r1))
            return Sign::common_plane;

        if (check_common_vertice(sign_plane2_p1, sign_plane2_q1, sign_plane2_r1))
            return Sign::common_vertice_other_poz_or_neg;

        auto sign_plane1_p2 = orient_3d(vertices_[0], vertices_[1], vertices_[2], vertices_2[0]);
        auto sign_plane1_q2 = orient_3d(vertices_[0], vertices_[1], vertices_[2], vertices_2[1]);
        auto sign_plane1_r2 = orient_3d(vertices_[0], vertices_[1], vertices_[2], vertices_2[2]);

        if (cmp::pozitive(sign_plane1_p2) && cmp::pozitive(sign_plane1_r2) &&
            cmp::pozitive(sign_plane1_q2))
            return Sign::pozitive;

        if (cmp::negative(sign_plane1_p2) && cmp::negative(sign_plane1_r2) &&
            cmp::negative(sign_plane1_q2))
            return Sign::negative;

        if (cmp::is_zero(sign_plane1_p2), cmp::is_zero(sign_plane1_r2),
            cmp::is_zero(sign_plane1_r2))
            return Sign::common_plane;

        if (check_common_vertice(sign_plane1_p2, sign_plane1_q2, sign_plane1_r2))
            return Sign::common_vertice_other_poz_or_neg;

        return Sign::different;
    }

    Sign check_relative_positions_2d(const Point &p, const Point &A, const Point &B, const Point &C,
                                     const Vector &n) const {
        double s1   = orient_2d(A, B, p, n);
        double s2   = orient_2d(B, C, p, n);
        double s3   = orient_2d(C, A, p, n);

        Sign   sign = Sign::different;

        if (s1 >= -cmp::float_eps && s2 >= -cmp::float_eps && s3 >= -cmp::float_eps)
            sign = Sign::pozitive;

        if (s1 <= cmp::float_eps && s2 <= cmp::float_eps && s3 <= cmp::float_eps)
            sign = Sign::negative;

        return sign;
    }

    bool on_segment_in_plane(const Point &a, const Point &b, const Point &p,
                             const Vector &n) const {
        if (std::abs(orient_2d(a, b, p, n)) > cmp::float_eps)
            return false;

        Vector ab = Vector(a, b);
        Vector ap = Vector(a, p);

        double t  = scalar_product(ap, ab);
        double L2 = scalar_product(ab, ab);

        if (t < -cmp::float_eps)
            return false;
        if (t > L2 + cmp::float_eps)
            return false;

        return true;
    }

    bool check_interval_intersect_2d(const Point &a, const Point &b, const Point &c, const Point &d,
                                     const Vector &n) const {
        double o1        = orient_2d(a, b, c, n);
        double o2        = orient_2d(a, b, d, n);
        double o3        = orient_2d(c, d, a, n);
        double o4        = orient_2d(c, d, b, n);

        bool   straddle1 = (o1 > cmp::float_eps && o2 < -cmp::float_eps) ||
                         (o1 < -cmp::float_eps && o2 > cmp::float_eps);
        bool straddle2 = (o3 > cmp::float_eps && o4 < -cmp::float_eps) ||
                         (o3 < -cmp::float_eps && o4 > cmp::float_eps);
        if (straddle1 && straddle2)
            return true;

        if (std::abs(o1) <= cmp::float_eps && on_segment_in_plane(a, b, c, n))
            return true;
        if (std::abs(o2) <= cmp::float_eps && on_segment_in_plane(a, b, d, n))
            return true;
        if (std::abs(o3) <= cmp::float_eps && on_segment_in_plane(c, d, a, n))
            return true;
        if (std::abs(o4) <= cmp::float_eps && on_segment_in_plane(c, d, b, n))
            return true;

        return false;
    }
};

inline Triangle canonicalize_triangle(const Triangle &base, const Triangle &ref) {
    base.print();
    std::array<double, 3> signs;
    auto                  canon          = base;
    auto                  vertices_canon = canon.get_vertices();
    auto                  vertices_ref   = ref.get_vertices();

    signs[0] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_canon[0]);
    signs[1] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_canon[1]);
    signs[2] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_canon[2]);

    if (cmp::pozitive(signs[0]) && signs[1] < -float_eps && signs[2] < -float_eps) {
        std::cout << "ret canon\n";
        return canon;
    }

    else if (signs[0] < -float_eps && cmp::pozitive(signs[1]) && signs[2] < -float_eps) {
        std::cout << "one rotate\n";
        canon.rotate_vertices();
    }

    else if (signs[0] < -float_eps && signs[1] < -float_eps && signs[2] > float_eps) {
        std::cout << "two rotates\n";
        canon.rotate_vertices();
        canon.rotate_vertices();
    }

    signs[0] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_canon[0]);
    signs[1] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_canon[1]);
    signs[2] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_canon[2]);

    if (signs[0] < -float_eps) {

        canon.swap_vertices(1, 2);
    }
    canon.print();

    return canon;
}
} // namespace triangle
#endif // INCLUDE_TRIANGLE_HPP
