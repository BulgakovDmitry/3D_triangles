#ifndef INCLUDE_PRIMITIVES_VECTOR_HPP
#define INCLUDE_PRIMITIVES_VECTOR_HPP

#include <cmath>
#include <ostream>
#include <stdexcept>

#include "common/cmp.hpp"
#include "point.hpp"

namespace triangle {

template <std::floating_point T> class Vector;

template <std::floating_point T> T scalar_product(const Vector<T> &v1, const Vector<T> &v2);

template <std::floating_point T> Vector<T> vector_product(const Vector<T> &v1, const Vector<T> &v2);

template <std::floating_point T>
T mixed_product(const Vector<T> &a, const Vector<T> &b, const Vector<T> &c);

template <std::floating_point T> class Vector {
  private:
    T x_;
    T y_;
    T z_;

  public:
    explicit Vector(const Point<T> &a, const Point<T> &b)
        : x_(b.get_x() - a.get_x()), y_(b.get_y() - a.get_y()), z_(b.get_z() - a.get_z()) {}

    explicit Vector(T x, T y, T z) : x_(x), y_(y), z_(z) {}

    explicit Vector(Point<T> p) : x_(p.get_x()), y_(p.get_y()), z_(get_z()) {} // radius-vector r_p

    Vector operator*(T k) const noexcept { return Vector(x_ * k, y_ * k, z_ * k); }
    Vector operator/(T k) const noexcept { return Vector(x_ / k, y_ / k, z_ / k); }
    Vector operator+(const Vector &v) const noexcept {
        return Vector(x_ + v.get_x(), y_ + v.get_y(), z_ + v.get_z());
    }
    Vector operator-(const Vector &v) const noexcept {
        return Vector(x_ - v.get_x(), y_ - v.get_y(), z_ - v.get_z());
    }

    T get_x() const noexcept { return x_; }
    T get_y() const noexcept { return y_; }
    T get_z() const noexcept { return z_; }

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
    return v1.get_x() * v2.get_x() + v1.get_y() * v2.get_y() + v1.get_z() * v2.get_z();
}

template <std::floating_point T>
Vector<T> vector_product(const Vector<T> &v1, const Vector<T> &v2) {

    const T x1 = v1.get_x(), y1 = v1.get_y(), z1 = v1.get_z();
    const T x2 = v2.get_x(), y2 = v2.get_y(), z2 = v2.get_z();

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
