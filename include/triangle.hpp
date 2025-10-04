#ifndef INCLUDE_TRIANGLE_HPP
#define INCLUDE_TRIANGLE_HPP

#include <array>
#include <iostream>
#include <utility>

#include "../common/cmp.hpp"
#include "BVH/AABB.hpp"
#include "primitives/point.hpp"
#include "primitives/vector.hpp"
#include <algorithm>
#include <array>
#include <span>
#include <utility>

namespace triangle {

enum class Sign {
    different,
    pozitive,
    negative,
    common_vertice_other_poz_or_neg,
    common_plane,
};

enum class TypeTriangle {
    triangle,
    interval,
    point,
};

template <std::floating_point T>
static bool check_common_vertice(T sign_plane_p, T sign_plane_r, T sign_plane_q) {
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
static size_t get_common_vertice(T sign_plane_p, T sign_plane_r, T sign_plane_q) {
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

class Triangle;

inline void update_sign_orient(const Triangle &base, const Triangle &ref,
                               std::array<double, 3> &signs);

inline float orient_3d(const Point &p_1, const Point &q_1, const Point &r_1, const Point &p_2) {
    Vector p_q(q_1.get_x() - p_1.get_x(), q_1.get_y() - p_1.get_y(), q_1.get_z() - p_1.get_z());
    Vector p_r(r_1.get_x() - p_1.get_x(), r_1.get_y() - p_1.get_y(), r_1.get_z() - p_1.get_z());
    Vector p_p(p_2.get_x() - p_1.get_x(), p_2.get_y() - p_1.get_y(), p_2.get_z() - p_1.get_z());

    return mixed_product(p_q, p_r, p_p);
}

inline float orient_2d(const Point &a, const Point &b, const Point &c, const Vector &n) {
    return mixed_product(Vector(a, b), Vector(a, c), n);
}

static bool is_in_plane(const Point &point, const Triangle &triangle);

inline bool point_inside_triangle(const Triangle &triangle, const Point &point);

inline bool interval_intersect_triangle(const Triangle &triangle, const Triangle &interval);

inline bool are_collinear(const Point &point_0, const Point &point_1, const Point &point_2) {
    Vector v1{point_1, point_0};
    Vector v2{point_2, point_0};
    Vector cross = vector_product(v1, v2);

    return cmp::is_zero(cross.get_x()) && cmp::is_zero(cross.get_y()) &&
           cmp::is_zero(cross.get_z());
}

Triangle canonicalize_triangle(const Triangle &base, const Triangle &ref);

class Triangle {
private:
    Point          vertices_[3];
    TypeTriangle   type_ = TypeTriangle::triangle;
    bin_tree::AABB box_;

  public:
    Triangle(const Point &point_0, const Point &point_1, const Point &point_2)
        : vertices_{point_0, point_1, point_2},
          box_(
              Point(std::min({vertices_[0].get_x(), vertices_[1].get_x(), vertices_[2].get_x()}),
                    std::min({vertices_[0].get_y(), vertices_[1].get_y(), vertices_[2].get_y()}),
                    std::min({vertices_[0].get_z(), vertices_[1].get_z(), vertices_[2].get_z()})),
              Point(std::max({vertices_[0].get_x(), vertices_[1].get_x(), vertices_[2].get_x()}),
                    std::max({vertices_[0].get_y(), vertices_[1].get_y(), vertices_[2].get_y()}),
                    std::max({vertices_[0].get_z(), vertices_[1].get_z(), vertices_[2].get_z()}))) {
        if (point_0 == point_1 && point_1 == point_2)
            type_ = TypeTriangle::point;

        else if (are_collinear(point_0, point_1, point_2))
            type_ = TypeTriangle::interval;
    }

    const Point (&get_vertices() const noexcept)[3] { return vertices_; }

    const TypeTriangle             &get_type() const noexcept { return type_; }

    const std::pair<size_t, size_t> get_interval() const noexcept {
        if (type_ != TypeTriangle::interval)
            return std::make_pair(0, 0);

        auto dist = [](const Point &a, const Point &b) {
            double dx = a.get_x() - b.get_x();
            double dy = a.get_y() - b.get_y();
            double dz = a.get_z() - b.get_z();
            return dx * dx + dy * dy + dz * dz;
        };

        double d01 = dist(vertices_[0], vertices_[1]);
        double d02 = dist(vertices_[0], vertices_[2]);
        double d12 = dist(vertices_[1], vertices_[2]);

        if (d01 >= d02 && d01 >= d12) {
            return {0, 1};
        } else if (d02 >= d01 && d02 >= d12) {
            return {0, 2};
        } else {
            return {1, 2};
        }
    }

    bool intersect(const Triangle &triangle) const {
        if (type_ == TypeTriangle::point)
            return point_inside_triangle(triangle, vertices_[0]);
        if (triangle.get_type() == TypeTriangle::point)
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

    bool intersect_2d(const Triangle &other) const {
        const auto &A = get_vertices();
        const auto &B = other.get_vertices();

        Vector n = vector_product(Vector(A[0], A[1]), Vector(A[0], A[2]));

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

    void rotate_clockwise() {
        auto copy = vertices_[2];
        vertices_[2] = vertices_[1];
        vertices_[1] = vertices_[0];
        vertices_[0] = copy;
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

    bin_tree::AABB get_box() const noexcept { return box_; }

  private:
    bool intersect_one_vertice_in_plane(const Triangle &triangle) const {
        size_t common_vertex;
        std::array<double, 3> signs;

        update_sign_orient(*this, triangle, signs);

        if (check_common_vertice(signs[0], signs[1], signs[2])) {
            common_vertex = get_common_vertice(signs[0], signs[1], signs[2]);
            if (point_inside_triangle(triangle, vertices_[common_vertex]))
                return true;
            return false;
        }

        update_sign_orient(triangle, *this, signs);

        if (check_common_vertice(signs[0], signs[1], signs[2])) {
            common_vertex = get_common_vertice(signs[0], signs[1], signs[2]);
            auto vertices_2 = triangle.get_vertices();
            if (point_inside_triangle(*this, vertices_2[common_vertex]))
                return true;
        }

        return false;
    }

    bool check_interval_intersect(const Triangle &canon_main, const Triangle &canon_ref) const {
        auto vertices_main = canon_main.get_vertices();
        auto vertices_ref = canon_ref.get_vertices();

        auto sign_1 =
            orient_3d(vertices_main[0], vertices_main[1], vertices_ref[0], vertices_ref[1]);
        auto sign_2 =
            orient_3d(vertices_main[0], vertices_main[2], vertices_ref[2], vertices_ref[0]);

        if (cmp::pozitive(sign_1) && cmp::pozitive(sign_2))
            return true;

        if (cmp::negative(sign_1) && cmp::negative(sign_2))
            return true;

        return false;
    }

    Sign check_relative_positions(const Triangle &triangle) const {
        std::array<double, 3> signs;

        update_sign_orient(*this, triangle, signs);

        if (cmp::pozitive(signs[0]) && cmp::pozitive(signs[1]) && cmp::pozitive(signs[2]))
            return Sign::pozitive;

        if (cmp::negative(signs[0]) && cmp::negative(signs[1]) && cmp::negative(signs[2]))

            return Sign::negative;

        if (cmp::is_zero(signs[0]) && cmp::is_zero(signs[1]) && cmp::is_zero(signs[2]))
            return Sign::common_plane;

        if (check_common_vertice(signs[0], signs[1], signs[2]))
            return Sign::common_vertice_other_poz_or_neg;

        update_sign_orient(triangle, *this, signs);

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

    Sign check_relative_positions_2d(const Point &p, const Point &A, const Point &B, const Point &C,
                                     const Vector &n) const {
        double s1 = orient_2d(A, B, p, n);
        double s2 = orient_2d(B, C, p, n);
        double s3 = orient_2d(C, A, p, n);

        Sign sign = Sign::different;

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

        double t = scalar_product(ap, ab);
        double L2 = scalar_product(ab, ab);

        if (t < -cmp::float_eps)
            return false;
        if (t > L2 + cmp::float_eps)
            return false;

        return true;
    }

    bool check_interval_intersect_2d(const Point &a, const Point &b, const Point &c, const Point &d,
                                     const Vector &n) const {
        double o1 = orient_2d(a, b, c, n);
        double o2 = orient_2d(a, b, d, n);
        double o3 = orient_2d(c, d, a, n);
        double o4 = orient_2d(c, d, b, n);

        bool straddle1 = (o1 > float_constants::float_eps && o2 < -float_constants::float_eps) ||
                         (o1 < -float_constants::float_eps && o2 > float_constants::float_eps);
        bool straddle2 = (o3 > float_constants::float_eps && o4 < -float_constants::float_eps) ||
                         (o3 < -float_constants::float_eps && o4 > float_constants::float_eps);
        if (straddle1 && straddle2)
            return true;

        if (std::abs(o1) <= float_constants::float_eps && on_segment_in_plane(a, b, c, n))
            return true;
        if (std::abs(o2) <= float_constants::float_eps && on_segment_in_plane(a, b, d, n))
            return true;
        if (std::abs(o3) <= float_constants::float_eps && on_segment_in_plane(c, d, a, n))
            return true;
        if (std::abs(o4) <= float_constants::float_eps && on_segment_in_plane(c, d, b, n))
            return true;

        return false;
    }
};

static bool is_in_plane(const Point &point, const Triangle &triangle) {
    auto   vertices = triangle.get_vertices();

    Vector normal =
        vector_product(Vector{vertices[1], vertices[0]}, Vector{vertices[2], vertices[0]});
    Vector to_point{point, vertices[0]};
    return cmp::is_zero(scalar_product(normal, to_point));
}

inline bool point_inside_triangle(const Triangle &triangle, const Point &point) {
    if (!is_in_plane(point, triangle))
        return false;

    auto vertices = triangle.get_vertices();

    // Calculate vectors from the vertices of the triangle to the point
    Vector v0(vertices[1], vertices[0]);
    Vector v1(vertices[2], vertices[0]);
    Vector v2(point, vertices[0]);

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

inline bool check_segments_intersect_2d(const Point &a, const Point &b, const Point &c,
                                        const Point &d) {
    auto orient_2d_simple = [](const Point &p, const Point &q, const Point &r) -> double {
        return (q.get_x() - p.get_x()) * (r.get_y() - p.get_y()) -
               (q.get_y() - p.get_y()) * (r.get_x() - p.get_x());
    };

    double o1 = orient_2d_simple(a, b, c);
    double o2 = orient_2d_simple(a, b, d);
    double o3 = orient_2d_simple(c, d, a);
    double o4 = orient_2d_simple(c, d, b);

    // Common intersection (the segments intersect)
    if (((o1 > cmp::float_eps && o2 < -cmp::float_eps) ||
         (o1 < -cmp::float_eps && o2 > cmp::float_eps)) &&
        ((o3 > cmp::float_eps && o4 < -cmp::float_eps) ||
         (o3 < -cmp::float_eps && o4 > cmp::float_eps)))
        return true;

    // Collinear cases - the point lies on the segment
    auto on_segment = [](const Point &p, const Point &q, const Point &r) -> bool {
        return (r.get_x() <= std::max(p.get_x(), q.get_x()) + cmp::float_eps &&
                r.get_x() >= std::min(p.get_x(), q.get_x()) - cmp::float_eps &&
                r.get_y() <= std::max(p.get_y(), q.get_y()) + cmp::float_eps &&
                r.get_y() >= std::min(p.get_y(), q.get_y()) - cmp::float_eps);
    };

    if (std::abs(o1) <= cmp::float_eps && on_segment(a, b, c))
        return true;
    if (std::abs(o2) <= cmp::float_eps && on_segment(a, b, d))
        return true;
    if (std::abs(o3) <= cmp::float_eps && on_segment(c, d, a))
        return true;
    if (std::abs(o4) <= cmp::float_eps && on_segment(c, d, b))
        return true;

    return false;
}

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

inline bool interval_intersect_triangle(const Triangle &triangle, const Triangle &interval) {
    std::pair<size_t, size_t> interval_ends     = interval.get_interval();

    auto                      interval_vertices = interval.get_vertices();
    auto                      int_start         = interval_vertices[interval_ends.first];
    auto                      int_end           = interval_vertices[interval_ends.second];

    auto                      vertices          = triangle.get_vertices();

    Vector                    edge_1{vertices[0], vertices[1]};
    Vector                    edge_2{vertices[0], vertices[2]};
    Vector                    normal = vector_product(edge_1, edge_2);

    // direct interval
    Vector                    int_dir{int_start, int_end};

    // check parallel
    float                     denom = scalar_product(normal, int_dir);
    if (cmp::is_zero(denom)) {
        // check whether the segment lies in the plane of the triangle
        if (is_in_plane(int_start, triangle) && is_in_plane(int_end, triangle))
            return check_segment_triangle_intersection_2d(int_start, int_end, triangle);

        return false; // interval is parallel of plane triangle
    }

    // calculate parameter t of intersect with plane
    Vector to_triangle{int_start, vertices[0]};
    float  t = scalar_product(normal, to_triangle) / denom;

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

inline void update_sign_orient(const Triangle &base, const Triangle &ref,
                               std::array<double, 3> &signs) {
    auto vertices_base = base.get_vertices();
    auto vertices_ref = ref.get_vertices();

    signs[0] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_base[0]);
    signs[1] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_base[1]);
    signs[2] = orient_3d(vertices_ref[0], vertices_ref[1], vertices_ref[2], vertices_base[2]);
}

inline Triangle canonicalize_triangle(const Triangle &base, const Triangle &ref) {
    std::array<double, 3> signs;
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

inline bin_tree::AABB calculate_bounding_box(const std::span<Triangle> &triangles) {
    bin_tree::AABB box;
    for (const auto &tr : triangles)
        box.wrap_in_box_with(tr.get_box());

    return box;
}

} // namespace triangle

#endif // INCLUDE_TRIANGLE_HPP
