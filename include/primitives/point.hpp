#ifndef INCLUDE_PRIMITIVES_POINT_HPP
#define INCLUDE_PRIMITIVES_POINT_HPP

#include <cmath>
#include <cstddef>
#include <ostream>
#include <stdexcept>

#include "common/cmp.hpp"

class Point {
  private:
    float x_;
    float y_;
    float z_;

  public:
    explicit Point(float x, float y, float z) : x_(x), y_(y), z_(z) {}

    float get_x() const noexcept { return x_; }
    float get_y() const noexcept { return y_; }
    float get_z() const noexcept { return z_; }

    float operator[](std::size_t i) const {
        switch (i) {
        case 0:
            return x_;
        case 1:
            return y_;
        case 2:
            return z_;
        default:
            throw std::out_of_range("Point index");
        }
    }
    float &operator[](std::size_t i) {
        switch (i) {
        case 0:
            return x_;
        case 1:
            return y_;
        case 2:
            return z_;
        default:
            throw std::out_of_range("Point index");
        }
    }

    void print(std::ostream &os) const { os << "point (" << x_ << ", " << y_ << ", " << z_ << ')'; }

    bool valid() const { return !(std::isnan(x_) || std::isnan(y_) || std::isnan(z_)); }

    bool operator==(const Point &p) const {
        return !cmp::fltcmp(x_, p.x_) && !cmp::fltcmp(y_, p.y_) && !cmp::fltcmp(z_, p.z_);
    }
};

#endif // INCLUDE_PRIMITIVES_POINT_HPP
