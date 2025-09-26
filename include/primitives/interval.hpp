#ifndef INCLUDE_PRIMITIVES_INTERVAL_HPP
#define INCLUDE_PRIMITIVES_INTERVAL_HPP

#include "../common/cmp.hpp"
#include "../common/colors.hpp"
#include "line.hpp"
#include "point.hpp"
#include "vector.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

class Interval {
private:
    Point p_min_, p_max_;

public:
    explicit Interval(const Line &l, const std::vector<Point> &points) {
        Vector a  = l.get_a();
        Vector r0 = l.get_r0();

        if (a.is_nul())
            throw std::runtime_error("Interval: line direction is zero");

        Vector u = a.normalize();

        if (points.empty()) {
            p_min_ = Point(r0.get_x(), r0.get_y(), r0.get_z());
            p_max_ = p_min_;
            return;
        }

        float s_min = std::numeric_limits<float>::infinity();
        float s_max = -std::numeric_limits<float>::infinity();

        for (const Point &p : points) {
            Vector op(p.get_x(), p.get_y(), p.get_z());
            Vector r    = op - r0;

            Vector proj = r.projection(a);

            float  s    = scalar_product(proj, u);

            if (s < s_min)
                s_min = s;
            if (s > s_max)
                s_max = s;
        }

        Vector v_min = r0 + u * s_min;
        Vector v_max = r0 + u * s_max;

        p_min_       = Point(v_min.get_x(), v_min.get_y(), v_min.get_z());
        p_max_       = Point(v_max.get_x(), v_max.get_y(), v_max.get_z());
    }

    explicit Interval(const Point &p_min, const Point &p_max) : p_min_(p_min), p_max_(p_max) {}

    bool valid() const { return (p_min_.valid() && p_max_.valid()); }

    void print() const {
        std::cout << BLUE << "interval " << CEAN << "{\n" << RESET;
        std::cout << "   ";
        p_max_.print();
        std::cout << "   ";
        p_min_.print();
        std::cout << CEAN << '}' << RESET << std::endl;
    }

    Point get_p_min() const noexcept { return p_min_; }
    Point get_p_max() const noexcept { return p_max_; }

    bool  is_nul() const noexcept {
        Point  O(0, 0, 0);
        Vector r_min(O, p_min_);
        Vector r_max(O, p_max_);

        return (r_max - r_min).is_nul();
    }

    /// TODO return true if intervals have intersection (used p_min and p_max)
    // bool intersect(const Interval& other) const {

    // }

    void erase() noexcept {
        p_max_.erase();
        p_min_.erase();
    }
};

#endif // INCLUDE_PRIMITIVES_INTERVAL_HPP