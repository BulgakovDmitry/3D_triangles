#ifndef INCLUDE_PRIMITIVES_POINT_HPP
#define INCLUDE_PRIMITIVES_POINT_HPP

#include "../../common/cmp.hpp"
#include "../../common/colors.hpp"
#include <iostream>

class Point {
private:
    float x_ = NAN;
    float y_ = NAN;
    float z_ = NAN;

public:
    Point() = default;
    explicit Point(float x, float y, float z) : x_(x), y_(y), z_(z) {}

    float get_x() const noexcept { return x_; }
    float get_y() const noexcept { return y_; }
    float get_z() const noexcept { return z_; }

    void  print() const {
        std::cout << BLUE << "point" << CEAN << '(' << MANG << x_ << GREEN << ", " << MANG << y_
                  << GREEN << ", " << MANG << z_ << CEAN << ')' << RESET << std::endl;
    }

    bool valid() const { return !(std::isnan(x_) || std::isnan(y_) || std::isnan(z_)); }

    bool operator==(const Point &p) const {
        return !cmp::fltcmp(x_, p.x_) && !cmp::fltcmp(y_, p.y_) && !cmp::fltcmp(z_, p.z_);
    }

    void erase() noexcept {
        x_ = NAN;
        y_ = NAN;
        z_ = NAN;
    }
};

#endif // INCLUDE_PRIMITIVES_POINT_HPP
