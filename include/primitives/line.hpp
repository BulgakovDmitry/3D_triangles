#ifndef INCLUDE_PRIMITIVES_LINE_HPP
#define INCLUDE_PRIMITIVES_LINE_HPP

#include "../common/cmp.hpp"
#include "../common/colors.hpp"
#include "point.hpp"
#include "vector.hpp"
#include <iostream>
#include <stdexcept>

class Line { // r = r0_ + t*a_
private:
    Vector a_;
    Vector r0_;

public:
    explicit Line(const Point &a, const Point &b)
        : a_(Vector(a, b)), r0_(Vector(Point(0, 0, 0), a)) {}
    explicit Line(const Point &p, const Vector &a) : a_(a), r0_(Vector(Point(0, 0, 0), p)) {}
    explicit Line(const Vector &r0, const Vector &a) : a_(a), r0_(r0) {}

    Vector get_a() const noexcept { return a_; }
    Vector get_r0() const noexcept { return r0_; }

    void   print() const {
        std::cout << BLUE << "line r = r0 + at\n" << BLUE << "   r0:\t" << RESET;
        r0_.print();

        std::cout << BLUE << "   a:\t" << RESET;
        a_.print();
    }

    bool valid() const { return a_.valid() && r0_.valid(); }

    bool contains(const Point &p) const {
        Vector OA = this->get_r0();
        Vector OP(p.get_x(), // X
                  p.get_y(), // Y
                  p.get_z()  // Z
        );
        Vector AP(OP.get_x() - OA.get_x(), // X
                  OP.get_y() - OA.get_y(), // Y
                  OP.get_z() - OA.get_z()  // Z
        );
        return this->get_a().collinear(AP);
    }

    bool contains(const Vector &OP) const {
        Vector OA = this->get_r0();
        Vector AP(OP.get_x() - OA.get_x(), // X
                  OP.get_y() - OA.get_y(), // Y
                  OP.get_z() - OA.get_z()  // Z
        );
        return this->get_a().collinear(AP);
    }

    bool collinear(const Line &l) const { return this->get_a().collinear(l.get_a()); }
    bool orthogonal(const Line &l) const { return this->get_a().orthogonal(l.get_a()); }

    bool operator==(const Line &l) const {
        if (!this->collinear(l))
            return false;

        return this->contains(l.get_r0());
    }
    bool operator!=(const Line &l) const { return !(*this == l); }

    void erase() noexcept {
        a_.erase();
        r0_.erase();
    }
};

#endif // INCLUDE_PRIMITIVES_LINE_HPP