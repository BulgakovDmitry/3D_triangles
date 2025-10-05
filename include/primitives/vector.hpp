#ifndef INCLUDE_PRIMITIVES_VECTOR_HPP
#define INCLUDE_PRIMITIVES_VECTOR_HPP

#include "common/cmp.hpp"
#include "point.hpp"
#include <iostream>
#include <stdexcept>

class Vector;

float scalar_product(const Vector &v1, const Vector &v2);
Vector vector_product(const Vector &v1, const Vector &v2);
float mixed_product(const Vector &a, const Vector &b, const Vector &c);

class Vector {
  private:
    float x_ = NAN;
    float y_ = NAN;
    float z_ = NAN;

  public:
    explicit Vector(const Point &a, const Point &b)
        : x_(b.get_x() - a.get_x()), y_(b.get_y() - a.get_y()), z_(b.get_z() - a.get_z()) {}

    explicit Vector(float x, float y, float z) : x_(x), y_(y), z_(z) {}

    explicit Vector(Point p) : x_(p.get_x()), y_(p.get_y()), z_(get_z()) {} // radius-vector r_p

    Vector operator*(float k) const { return Vector(x_ * k, y_ * k, z_ * k); }
    Vector operator/(float k) const { return Vector(x_ / k, y_ / k, z_ / k); }
    Vector operator+(const Vector &v) const {
        return Vector(x_ + v.get_x(), y_ + v.get_y(), z_ + v.get_z());
    }
    Vector operator-(const Vector &v) const {
        return Vector(x_ - v.get_x(), y_ - v.get_y(), z_ - v.get_z());
    }

    float get_x() const noexcept { return x_; }
    float get_y() const noexcept { return y_; }
    float get_z() const noexcept { return z_; }

    void print() const {
        std::cout << "vector {" << x_ << ", " << y_
                  << ", " << z_ << "}\n";
    }

    bool valid() const { return !(std::isnan(x_) || std::isnan(y_) || std::isnan(z_)); }
    bool is_nul() const noexcept { return cmp::fltcmp(scalar_product(*this, *this), 0) == 0; }

    float abs() const noexcept { return static_cast<float>(sqrt(scalar_product(*this, *this))); }

    Vector normalize() const noexcept {
        if (this->is_nul())
            return Vector(0, 0, 0);

        return (*this) / this->abs();
    }

    Vector projection(const Vector &onto) const {
        if (!onto.valid())
            throw std::runtime_error("it is impossible to project");

        float numerator = scalar_product(*this, onto);
        float denominator = scalar_product(onto, onto);

        if (cmp::fltcmp(denominator, 0) == 0)
            return Vector(0, 0, 0);

        return onto * (numerator / denominator);
    }

    bool collinear(const Vector &v) const {
        Vector ret = vector_product(*this, v);
        return ret.is_nul();
    }

    bool orthogonal(const Vector &v) const { return (!cmp::fltcmp(scalar_product(*this, v), 0)); }
};

// --------------------------------------------------------------------------------------
//                           mathematical functions
// --------------------------------------------------------------------------------------

inline float scalar_product(const Vector &v1, const Vector &v2) {
    if (!v1.valid() || !v2.valid())
        throw std::runtime_error("Invalid input vectors");

    return v1.get_x() * v2.get_x() + v1.get_y() * v2.get_y() + v1.get_z() * v2.get_z();
}

inline Vector vector_product(const Vector &v1, const Vector &v2) {
    if (!v1.valid() || !v2.valid())
        throw std::runtime_error("Invalid input vectors");

    const float x1 = v1.get_x(), y1 = v1.get_y(), z1 = v1.get_z();
    const float x2 = v2.get_x(), y2 = v2.get_y(), z2 = v2.get_z();

    return Vector(y1 * z2 - z1 * y2, // X
                  z1 * x2 - x1 * z2, // Y
                  x1 * y2 - y1 * x2  // Z
    );
}

inline float mixed_product(const Vector &a, const Vector &b, const Vector &c) {
    return scalar_product(vector_product(a, b), c);
}

#endif // INCLUDE_PRIMITIVES_VECTOR_HPP
