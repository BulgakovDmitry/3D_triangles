#ifndef INCLUDE_TRIANGLE_HPP
#define INCLUDE_TRIANGLE_HPP

#include <array>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <utility>
#include <cassert>

#include "BVH/AABB.hpp"
#include "common/cmp.hpp"
#include "point.hpp"
#include "vector.hpp"

namespace triangle {

enum class TypeTriangle {
    triangle,
    interval,
    point,
};

template <std::floating_point T>
inline bool are_collinear(const Point<T> &point_0, const Point<T> &point_1,
                          const Point<T> &point_2) {
    Vector<T> v1{point_1, point_0};
    Vector<T> v2{point_2, point_0};
    Vector<T> cross = vector_product(v1, v2);

    return cross.is_nul();
}

template <std::floating_point T> class Triangle {
  private:
    using VerticesT = std::array<Point<T>, 3>;

    VerticesT vertices_;
    TypeTriangle type_ = TypeTriangle::triangle;
    bounding_box::AABB<T> box_;
    std::size_t id_;

  public:
    Triangle(const Point<T> &point_0, const Point<T> &point_1, const Point<T> &point_2,
             std::size_t id)
        : vertices_{point_0, point_1, point_2},
          box_(Point(std::min({vertices_[0].get_x(), vertices_[1].get_x(), vertices_[2].get_x()}),
                     std::min({vertices_[0].get_y(), vertices_[1].get_y(), vertices_[2].get_y()}),
                     std::min({vertices_[0].get_z(), vertices_[1].get_z(), vertices_[2].get_z()})),
               Point(std::max({vertices_[0].get_x(), vertices_[1].get_x(), vertices_[2].get_x()}),
                     std::max({vertices_[0].get_y(), vertices_[1].get_y(), vertices_[2].get_y()}),
                     std::max({vertices_[0].get_z(), vertices_[1].get_z(), vertices_[2].get_z()}))),
          id_(id) {
        if (point_0 == point_1 && point_1 == point_2)
            type_ = TypeTriangle::point;

        else if (are_collinear(point_0, point_1, point_2))
            type_ = TypeTriangle::interval;
    }

    Triangle(const Point<T> &point_0, const Point<T> &point_1, const Point<T> &point_2)
        : Triangle(point_0, point_1, point_2, 0) {}

    const VerticesT &get_vertices() const { return vertices_; }

    TypeTriangle get_type() const noexcept { return type_; }

    std::pair<size_t, size_t> get_interval() const noexcept {
        if (type_ != TypeTriangle::interval)
            return {0, 0};

        auto dist = [](const Point<T> &a, const Point<T> &b) {
            T dx = a.get_x() - b.get_x();
            T dy = a.get_y() - b.get_y();
            T dz = a.get_z() - b.get_z();
            return dx * dx + dy * dy + dz * dz;
        };

        T d01 = dist(vertices_[0], vertices_[1]);
        T d02 = dist(vertices_[0], vertices_[2]);
        T d12 = dist(vertices_[1], vertices_[2]);

        if (cmp::greater_or_equal(d01, d02) && cmp::greater_or_equal(d01, d12)) {
            return {0, 1};
        } else if (cmp::greater_or_equal(d02, d01) && cmp::greater_or_equal(d02, d12)) {
            return {0, 2};
        } else {
            return {1, 2};
        }
    }

    std::size_t get_id() const noexcept { return id_; }

    void rotate_clockwise() {
        auto copy = vertices_[2];
        vertices_[2] = vertices_[1];
        vertices_[1] = vertices_[0];
        vertices_[0] = copy;
    }
    void swap_vertices(std::size_t i, std::size_t j) { 
        assert(i < 3 && j < 3);
        std::swap(vertices_[i], vertices_[j]); 
    }

    void print(std::ostream &os) const {
        os << "triangle " << '{';
        for (auto &vertex : vertices_) {
            os << "   ";
            vertex.print(os);
        }
        os << '}';
    }

    bounding_box::AABB<T> get_box() const noexcept { return box_; }
};

} // namespace triangle

#endif // INCLUDE_TRIANGLE_HPP
