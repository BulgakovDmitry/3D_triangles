#ifndef INCLUDE_PRIMITIVES_POINT_HPP
#define INCLUDE_PRIMITIVES_POINT_HPP

#include <cmath>
#include <cstddef>
#include <ostream>
#include <stdexcept>

#include "common/cmp.hpp"

template <std::floating_point T> class Point {
  private:
    T x_;
    T y_;
    T z_;

  public:
    explicit Point(T x, T y, T z) : x_(x), y_(y), z_(z) {}

    T get_x() const noexcept { return x_; }
    T get_y() const noexcept { return y_; }
    T get_z() const noexcept { return z_; }

    void print(std::ostream &os) const { os << "point (" << x_ << ", " << y_ << ", " << z_ << ')'; }

    bool operator==(const Point<T> p) const {
        return !cmp::fltcmp(x_, p.x_) && !cmp::fltcmp(y_, p.y_) && !cmp::fltcmp(z_, p.z_);
    }
};

#endif // INCLUDE_PRIMITIVES_POINT_HPP
