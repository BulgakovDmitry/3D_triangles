#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "../common/cmp.hpp"
#include "../common/colors.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

class Point {
private:
    float x_ = NAN;
    float y_ = NAN;
    float z_ = NAN;

public:
    explicit Point(float x, float y, float z);

    float get_x() const noexcept;
    float get_y() const noexcept;
    float get_z() const noexcept;

    void  print() const;

    bool  valid() const;

    bool  equal(const Point &p);

    void  erase() noexcept;
};

class Vector {
private:
    float x_ = NAN;
    float y_ = NAN;
    float z_ = NAN;

public:
    explicit Vector(const Point &a, const Point &b);
    explicit Vector(float x, float y, float z);

    Vector operator*(float k) const;

    Vector operator/(float k) const;
    Vector operator+(const Vector &v) const;

    float  get_x() const noexcept;
    float  get_y() const noexcept;
    float  get_z() const noexcept;

    void   print() const;

    bool   valid() const;

    bool   is_nul() const noexcept;

    float  abs() const noexcept;

    Vector normalize() const noexcept;

    Vector projection(const Vector &onto) const;

    bool   collinear(const Vector &v) const;
    bool   orthogonal(const Vector &v) const;

    void   erase() noexcept;
};

class Line { // r = r0_ + t*a_
private:
    Vector a_;
    Vector r0_;

public:
    explicit Line(const Point &a, const Point &b);
    explicit Line(const Point &p, const Vector &a);
    explicit Line(const Vector &r0, const Vector &a);

    Vector get_a() const noexcept;
    Vector get_r0() const noexcept;

    void   print() const;

    bool   valid() const;

    bool   contains(const Point &p) const;
    bool   contains(const Vector &OP) const;

    bool   collinear(const Line &l) const;
    bool   orthogonal(const Line &l) const;
    bool   equal(const Line &l) const;

    void   erase() noexcept;
};

class Interval {
private:
    Point p_min_, p_max_; // points of start and end of interval

public:
    /// TODO construct interval as max projection to line
    explicit Interval(const Line &l, const std::vector<Point> &points);

    explicit Interval(const Point &p_min, const Point &p_max);

    bool                    valid() const;

    void                    print() const;

    std::pair<Point, Point> getPoints() const noexcept;

    /// TODO return true if intervals have intersection (used p_min and p_max)
    bool                    intersect(const Interval &interval) const;

    void                    erase() noexcept;
};

class Polygon {
private:
    std::vector<Point> vertices_;

public:
    Polygon();
    explicit Polygon(const std::vector<Point> &points);

    void  print() const;

    bool  valid() const;

    bool  contains(const Point &p) const;
    bool  contains(const Vector &OP) const; // OP = радиус-вектор точки p
    bool  contains(const Line &l) const;

    bool  complanar(const Polygon &pol) const;
    bool  orthogonal(const Polygon &pol) const;
    bool  equal(const Polygon &pol) const;

    float distance(const Point &p) const;

    Line  intersect(const Polygon &pol) const;

    void  erase() noexcept;
};

float  scalar_product(const Vector &v1, const Vector &v2);
Vector vector_product(const Vector &v1, const Vector &v2);

// --------------------------------------------------------------------------------------
//                           point class methods
// --------------------------------------------------------------------------------------

Point::Point(float x, float y, float z) : x_(x), y_(y), z_(z) {}

float Point::get_x() const noexcept { return x_; }
float Point::get_y() const noexcept { return y_; }
float Point::get_z() const noexcept { return z_; }

void  Point::print() const {
    std::cout << BLUE << "point" << CEAN << '(' << MANG << x_ << GREEN << ", " << MANG << y_
              << GREEN << ", " << MANG << z_ << CEAN << ')' << RESET << std::endl;
}

bool Point::valid() const { return !(std::isnan(x_) || std::isnan(y_) || std::isnan(z_)); }

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
    : x_(b.get_x() - a.get_x()), y_(b.get_y() - a.get_y()), z_(b.get_z() - a.get_z()) {}

Vector::Vector(float x, float y, float z) : x_(x), y_(y), z_(z) {}

Vector Vector::operator*(float k) const { return Vector(x_ * k, y_ * k, z_ * k); }

Vector Vector::operator/(float k) const { return Vector(x_ / k, y_ / k, z_ / k); }

Vector Vector::operator+(const Vector &v) const {
    return Vector(x_ + v.get_x(), y_ + v.get_y(), z_ + v.get_z());
}

float Vector::get_x() const noexcept { return x_; }
float Vector::get_y() const noexcept { return y_; }
float Vector::get_z() const noexcept { return z_; }

void  Vector::print() const {
    std::cout << BLUE << "vector" << CEAN << '{' << MANG << x_ << GREEN << ", " << MANG << y_
              << GREEN << ", " << MANG << z_ << CEAN << '}' << RESET << std::endl;
}

bool  Vector::valid() const { return !(std::isnan(x_) || std::isnan(y_) || std::isnan(z_)); }

bool  Vector::is_nul() const noexcept { return !fltcmp(x_ * x_ + y_ * y_ + z_ * z_, 0); }

float Vector::abs() const noexcept {
    return static_cast<float>(sqrt(scalar_product(*this, *this)));
}

Vector Vector::normalize() const noexcept {
    if (this->is_nul())
        return Vector(0, 0, 0);

    return (*this) / this->abs();
}

Vector Vector::projection(const Vector &onto) const {
    if (!onto.valid())
        throw std::runtime_error("it is impossible to project");

    float numerator   = scalar_product(*this, onto);
    float denominator = scalar_product(onto, onto);

    if (fltcmp(denominator, 0) == 0)
        return Vector(0, 0, 0);

    return onto * (numerator / denominator);
}

bool Vector::collinear(const Vector &v) const {
    Vector ret = vector_product(*this, v);
    return ret.is_nul();
}

bool Vector::orthogonal(const Vector &v) const { return (!fltcmp(scalar_product(*this, v), 0)); }

void Vector::erase() noexcept {
    x_ = NAN;
    y_ = NAN;
    z_ = NAN;
}

// --------------------------------------------------------------------------------------
//                           line class methods
// --------------------------------------------------------------------------------------

Line::Line(const Point &a, const Point &b) : a_(Vector(a, b)), r0_(Vector(Point(0, 0, 0), a)) {}
Line::Line(const Point &p, const Vector &a) : a_(a), r0_(Vector(Point(0, 0, 0), p)) {}
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

bool Line::contains(const Vector &OP) const {
    Vector OA = this->get_r0();
    Vector AP(OP.get_x() - OA.get_x(), // X
              OP.get_y() - OA.get_y(), // Y
              OP.get_z() - OA.get_z()  // Z
    );
    return this->get_a().collinear(AP);
}

bool Line::collinear(const Line &l) const { return this->get_a().collinear(l.get_a()); }

bool Line::orthogonal(const Line &l) const { return this->get_a().orthogonal(l.get_a()); }

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
//                           interval class methods
// --------------------------------------------------------------------------------------

Interval::Interval(const Point &p_min, const Point &p_max) : p_min_(p_min), p_max_(p_max) {}
// Interval::Interval(const Line &l, const std::vector<Point> &points) { //TODO

// }

// --------------------------------------------------------------------------------------
//                           polygon class methods
// --------------------------------------------------------------------------------------

Polygon::Polygon() = default;
Polygon::Polygon(const std::vector<Point> &points) {
    if (points.size() > 6)
        throw std::invalid_argument("Polygon can have max 6 vertices");

    vertices_ = points;
}

void Polygon::print() const {
    std::cout << BLUE << "polygon " << CEAN << "{\n" << RESET;
    for (std::size_t i = 0; i < vertices_.size(); ++i) {
        std::cout << "   ";
        vertices_[i].print();
    }
    std::cout << CEAN << '}' << RESET << std::endl;
}

bool Polygon::valid() const {
    std::size_t vsz = vertices_.size();

    if (vsz == 0)
        return false;

    for (size_t i = 0; i < vsz; i++) {
        if (!vertices_[i].valid())
            return false;
    }

    return true;
}

void Polygon::erase() noexcept {
    std::size_t vsz = vertices_.size();

    for (std::size_t i = 0; i < vsz; ++i) {
        if (vertices_[i].valid())
            vertices_[i].erase();
    }
}

// --------------------------------------------------------------------------------------
//                           mathematical functions
// --------------------------------------------------------------------------------------

float scalar_product(const Vector &v1, const Vector &v2) {
    if (!v1.valid() || !v2.valid())
        throw std::runtime_error("Invalid input vectors");

    return v1.get_x() * v2.get_x() + v1.get_y() * v2.get_y() + v1.get_z() * v2.get_z();
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

#endif
