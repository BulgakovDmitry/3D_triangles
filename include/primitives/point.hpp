#ifndef INCLUDE_PRIMITIVES_POINT_HPP
#define INCLUDE_PRIMITIVES_POINT_HPP

#include <cmath>
#include <ostream>

#include "common/cmp.hpp"

namespace triangle {

template <std::floating_point T> struct Point {
    T x_;
    T y_;
    T z_;

    Point(T x, T y, T z) : x_(x), y_(y), z_(z) {}

    void print(std::ostream &os) const { os << "point (" << x_ << ", " << y_ << ", " << z_ << ')'; }

    bool operator==(const Point<T> &p) const {
        return !cmp::fltcmp(x_, p.x_) && !cmp::fltcmp(y_, p.y_) && !cmp::fltcmp(z_, p.z_);
    }
};

} // namespace triangle

#endif // INCLUDE_PRIMITIVES_POINT_HPP
