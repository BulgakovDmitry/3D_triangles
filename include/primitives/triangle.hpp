#ifndef INCLUDE_TRIANGLE_HPP
#define INCLUDE_TRIANGLE_HPP

#include <iostream>

#include "cmp.hpp"
#include "BVH/AABB.hpp"
#include "point.hpp"
#include "vector.hpp"
#include <algorithm>
#include <utility>

namespace triangle {

enum class TypeTriangle {
    triangle,
    interval,
    point,
};

inline bool are_collinear(const Point &point_0, const Point &point_1, const Point &point_2) {
    Vector v1{point_1, point_0};
    Vector v2{point_2, point_0};
    Vector cross = vector_product(v1, v2);

    return cmp::is_zero(cross.get_x()) && cmp::is_zero(cross.get_y()) &&
           cmp::is_zero(cross.get_z());
}

class Triangle {
  private:
    Point vertices_[3];
    TypeTriangle type_ = TypeTriangle::triangle;
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

    const TypeTriangle &get_type() const noexcept { return type_; }

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
};

} // namespace triangle

#endif // INCLUDE_TRIANGLE_HPP
