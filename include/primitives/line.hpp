#ifndef INCLUDE_PRIMITIVES_LINE_HPP
#define INCLUDE_PRIMITIVES_LINE_HPP

#include "common/cmp.hpp"
#include "point.hpp"
#include "vector.hpp"
#include <iostream>
#include <ostream>
#include <stdexcept>

namespace triangle {

template <std::floating_point T> class Line { // r = r0_ + t*a_
  private:
    Vector<T> a_;
    Vector<T> r0_;

  public:
    explicit Line(const Point<T> &a, const Point<T> &b)
        : a_(Vector<T>(a, b)), r0_(Vector<T>(Point<T>(0, 0, 0), a)) {}
    explicit Line(const Point<T> &p, const Vector<T> &a)
        : a_(a), r0_(Vector<T>(Point<T>(0, 0, 0), p)) {}
    explicit Line(const Vector<T> &r0, const Vector<T> &a) : a_(a), r0_(r0) {}

    Vector<T> get_a() const noexcept { return a_; }
    Vector<T> get_r0() const noexcept { return r0_; }

    void print(std::ostream &os) const {
        os << "line r = r0 + at\n"
           << "   r0:\t";
        r0_.print(os);

        os << "   a:\t";
        a_.print(os);
    }

    bool contains(const Point<T> &p) const {
        Vector<T> OA = this->get_r0();
        Vector<T> OP(p.get_x(), // X
                     p.get_y(), // Y
                     p.get_z()  // Z
        );
        Vector<T> AP = OP - OA;
        return get_a().collinear(AP);
    }

    bool contains(const Vector<T> &OP) const {
        Vector<T> OA = this->get_r0();
        Vector<T> AP(OP.get_x() - OA.get_x(), // X
                     OP.get_y() - OA.get_y(), // Y
                     OP.get_z() - OA.get_z()  // Z
        );
        return get_a().collinear(AP);
    }

    bool collinear(const Line<T> &l) const { return get_a().collinear(l.get_a()); }
    bool orthogonal(const Line<T> &l) const { return get_a().orthogonal(l.get_a()); }

    bool operator==(const Line<T> &l) const {
        if (!this->collinear(l))
            return false;

        return contains(l.get_r0());
    }
};

} // namespace triangle

#endif // INCLUDE_PRIMITIVES_LINE_HPP
