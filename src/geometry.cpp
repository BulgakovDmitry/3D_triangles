#include "../include/geometry.hpp"
#include "../common/cmp.hpp"
#include "../common/colors.hpp"
#include <iostream>
#include <stdexcept>

// --------------------------------------------------------------------------------------
//                           point class methods
// --------------------------------------------------------------------------------------

Point::Point(float x, float y, float z) : x_(x), y_(y), z_(z) {}

float Point::get_x() const noexcept { return x_; }
float Point::get_y() const noexcept { return y_; }
float Point::get_z() const noexcept { return z_; }

void  Point::print() const {
    std::cout << BLUE << "point" << CEAN << '(' << MANG << x_ << GREEN << ", "
              << MANG << y_ << GREEN << ", " << MANG << z_ << CEAN << ')'
              << RESET << std::endl;
}

bool Point::valid() const {
    return !(std::isnan(x_) || std::isnan(y_) || std::isnan(z_));
}

bool Point::equal(const Point &p) {
    return (!fltcmp(x_, p.x_)) && (!fltcmp(x_, p.x_)) && (!fltcmp(x_, p.x_));
}

void Point::erase() noexcept {
    x_ = NAN;
    y_ = NAN;
    z_ = NAN;
}

// --------------------------------------------------------------------------------------
//                           vector class methods
// --------------------------------------------------------------------------------------

Vector::Vector(const Point &a, const Point &b)
    : x_(b.get_x() - a.get_x()), y_(b.get_y() - a.get_y()),
      z_(b.get_z() - a.get_z()) {}

Vector::Vector(float x, float y, float z) : x_(x), y_(y), z_(z) {}

float Vector::get_x() const noexcept { return x_; }
float Vector::get_y() const noexcept { return y_; }
float Vector::get_z() const noexcept { return z_; }

void  Vector::print() const {
    std::cout << BLUE << "vector" << CEAN << '{' << MANG << x_ << GREEN << ", "
              << MANG << y_ << GREEN << ", " << MANG << z_ << CEAN << '}'
              << RESET << std::endl;
}

bool Vector::valid() const {
    return !(std::isnan(x_) || std::isnan(y_) || std::isnan(z_));
}

bool Vector::is_nul() const noexcept {
    return !fltcmp(x_ * x_ + y_ * y_ + z_ * z_, 0);
}

bool Vector::collinear(const Vector &v) const {
    Vector ret = vector_product(*this, v);
    return ret.is_nul();
}

bool Vector::orthogonal(const Vector &v) const {
    return (!fltcmp(scalar_product(*this, v), 0));
}

void Vector::erase() noexcept {
    x_ = NAN;
    y_ = NAN;
    z_ = NAN;
}

// --------------------------------------------------------------------------------------
//                           line class methods
// --------------------------------------------------------------------------------------

Line::Line(const Point &a, const Point &b)
    : a_(Vector(a, b)), r0_(Vector(Point(0, 0, 0), a)) {}
Line::Line(const Point &p, const Vector &a)
    : a_(a), r0_(Vector(Point(0, 0, 0), p)) {}
Line::Line(const Vector &r0, const Vector &a) : a_(a), r0_(r0) {}

Vector Line::get_a() const noexcept { return a_; }
Vector Line::get_r0() const noexcept { return r0_; }

void   Line::print() const {
    std::cout << BLUE << "line r = r0 + at\n" << BLUE << "   r0:\t" << RESET;
    r0_.print();

    std::cout << BLUE << "   a:\t" << RESET;
    a_.print();
}

bool Line::valid() const { return a_.valid() && r0_.valid(); }

bool Line::contains(const Point &p) const {
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

bool Line::contains(const Point &p) const {
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

bool Line::contains(const Point &OP) const {
    Vector OA = this->get_r0();
    Vector AP(OP.get_x() - OA.get_x(), // X
              OP.get_y() - OA.get_y(), // Y
              OP.get_z() - OA.get_z()  // Z
    );
    return this->get_a().collinear(AP);
}

bool Line::collinear(const Line &l) const {
    return this->get_a().collinear(l.get_a());
}

bool Line::orthogonal(const Line &l) const {
    return this->get_a().orthogonal(l.get_a());
}

bool Line::equal(const Line &l) const {
    if (!this->collinear(l))
        return false;

    return this->contains(l.get_r0());
}

void Line::erase() noexcept {
    a_.erase();
    r0_.erase();
}

// --------------------------------------------------------------------------------------
//                           mathematical functions
// --------------------------------------------------------------------------------------

float scalar_product(const Vector &v1, const Vector &v2) {
    if (!v1.valid() || !v2.valid())
        throw std::runtime_error("Invalid input vectors");

    return v1.get_x() * v2.get_x() + v1.get_y() * v2.get_y() +
           v1.get_z() * v2.get_z();
}

Vector vector_product(const Vector &v1, const Vector &v2) {
    if (!v1.valid() || !v2.valid())
        throw std::runtime_error("Invalid input vectors");

    const float x1 = v1.get_x(), y1 = v1.get_y(), z1 = v1.get_z();
    const float x2 = v2.get_x(), y2 = v2.get_y(), z2 = v2.get_z();

    return Vector(y1 * z2 - z1 * y2, // X
                  z1 * x2 - x1 * z2, // Y
                  x1 * y2 - y1 * x2  // Z
    );
}
