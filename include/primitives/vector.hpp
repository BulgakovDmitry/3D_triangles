#ifndef INCLUDE_PRIMITIVES_VECTOR_HPP
#define INCLUDE_PRIMITIVES_VECTOR_HPP

#include <cmath>
#include <ostream>

#include "common/cmp.hpp"
#include "point.hpp"

namespace triangle {

template <std::floating_point T> struct Vector;

template <std::floating_point T> T scalar_product(const Vector<T> &v1, const Vector<T> &v2);

template <std::floating_point T> Vector<T> vector_product(const Vector<T> &v1, const Vector<T> &v2);

template <std::floating_point T>
T mixed_product(const Vector<T> &a, const Vector<T> &b, const Vector<T> &c);

template <std::floating_point T> struct Vector {
    T x_;
    T y_;
    T z_;

    Vector(const Point<T> &a, const Point<T> &b)
        : x_(b.x_ - a.x_), y_(b.y_ - a.y_), z_(b.z_ - a.z_) {}

    Vector(T x, T y, T z) : x_(x), y_(y), z_(z) {}

    explicit Vector(Point<T> p) : x_(p.x_), y_(p.y_), z_(z_) {} // radius-vector r_p

    Vector operator*(T k) const noexcept {
        return Vector(x_ * k, y_ * k, z_ * k);
    } // TODO add *= += ...
    Vector operator/(T k) const noexcept { return Vector(x_ / k, y_ / k, z_ / k); }
    Vector operator+(const Vector &v) const noexcept {
        return Vector(x_ + v.x_, y_ + v.y_, z_ + v.z_);
    }
    Vector operator-(const Vector &v) const noexcept {
        return Vector(x_ - v.x_, y_ - v.y_, z_ - v.z_);
    }

    void print(std::ostream &os) const {
        os << "vector {" << x_ << ", " << y_ << ", " << z_ << '}';
    }

    bool is_nul() const { return cmp::fltcmp(scalar_product(*this, *this), (T)0) == 0; }

    T abs() const noexcept { return std::sqrt(scalar_product(*this, *this)); }

    Vector<T> normalize() const noexcept {
        if (is_nul())
            return Vector<T>(0, 0, 0);

        return (*this) / abs();
    }

    Vector projection(const Vector<T> &onto) const {

        T denominator = scalar_product(onto, onto);

        if (cmp::fltcmp(denominator, (T)0) == 0)
            return Vector<T>(0, 0, 0);

        T numerator = scalar_product(*this, onto);

        return onto * (numerator / denominator);
    }

    bool collinear(const Vector<T> &v) const { return vector_product(*this, v).is_nul(); }

    bool orthogonal(const Vector<T> &v) const {
        return (!cmp::fltcmp(scalar_product(*this, v), (T)0));
    }
};

// --------------------------------------------------------------------------------------
//                           mathematical functions
// --------------------------------------------------------------------------------------

template <std::floating_point T> T scalar_product(const Vector<T> &v1, const Vector<T> &v2) {
    return v1.x_ * v2.x_ + v1.y_ * v2.y_ + v1.z_ * v2.z_;
}

template <std::floating_point T>
Vector<T> vector_product(const Vector<T> &v1, const Vector<T> &v2) {

    const T x1 = v1.x_, y1 = v1.y_, z1 = v1.z_;
    const T x2 = v2.x_, y2 = v2.y_, z2 = v2.z_;

    return Vector(y1 * z2 - z1 * y2, // X
                  z1 * x2 - x1 * z2, // Y
                  x1 * y2 - y1 * x2  // Z
    );
}

template <std::floating_point T>
T mixed_product(const Vector<T> &a, const Vector<T> &b, const Vector<T> &c) {
    return scalar_product(vector_product(a, b), c);
}

} // namespace triangle

#endif // INCLUDE_PRIMITIVES_VECTOR_HPP
