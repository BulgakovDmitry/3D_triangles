#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "../common/cmp.hpp"
#include "../common/colors.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

class Vector;

float  scalar_product(const Vector &v1, const Vector &v2);
Vector vector_product(const Vector &v1, const Vector &v2);

class Point {
private:
    float x_ = NAN;
    float y_ = NAN;
    float z_ = NAN;

public:
    Point() = default;
    Point(float x, float y, float z) : x_(x), y_(y), z_(z) {}

    float get_x() const noexcept { return x_; }
    float get_y() const noexcept { return y_; }
    float get_z() const noexcept { return z_; }

    void  print() const {
        std::cout << BLUE << "point" << CEAN << '(' << MANG << x_ << GREEN << ", " << MANG << y_
                  << GREEN << ", " << MANG << z_ << CEAN << ')' << RESET << std::endl;
    }

    bool valid() const { return !(std::isnan(x_) || std::isnan(y_) || std::isnan(z_)); }

    bool operator==(const Point &p) const {
        return !fltcmp(x_, p.x_) && !fltcmp(y_, p.y_) && !fltcmp(z_, p.z_);
    }

    void erase() noexcept {
        x_ = NAN;
        y_ = NAN;
        z_ = NAN;
    }
};

class Vector {
private:
    float x_ = NAN;
    float y_ = NAN;
    float z_ = NAN;

public:
    explicit Vector(const Point &a, const Point &b)
        : x_(b.get_x() - a.get_x()), y_(b.get_y() - a.get_y()), z_(b.get_z() - a.get_z()) {}

    explicit Vector(float x, float y, float z) : x_(x), y_(y), z_(z) {}

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

    void  print() const {
        std::cout << BLUE << "vector" << CEAN << '{' << MANG << x_ << GREEN << ", " << MANG << y_
                  << GREEN << ", " << MANG << z_ << CEAN << '}' << RESET << std::endl;
    }

    bool   valid() const { return !(std::isnan(x_) || std::isnan(y_) || std::isnan(z_)); }
    bool   is_nul() const noexcept { return fltcmp(scalar_product(*this, *this), 0) == 0; }

    float  abs() const noexcept { return static_cast<float>(sqrt(scalar_product(*this, *this))); }

    Vector normalize() const noexcept {
        if (this->is_nul())
            return Vector(0, 0, 0);

        return (*this) / this->abs();
    }

    Vector projection(const Vector &onto) const {
        if (!onto.valid())
            throw std::runtime_error("it is impossible to project");

        float numerator   = scalar_product(*this, onto);
        float denominator = scalar_product(onto, onto);

        if (fltcmp(denominator, 0) == 0)
            return Vector(0, 0, 0);

        return onto * (numerator / denominator);
    }

    bool collinear(const Vector &v) const {
        Vector ret = vector_product(*this, v);
        return ret.is_nul();
    }

    bool orthogonal(const Vector &v) const { return (!fltcmp(scalar_product(*this, v), 0)); }

    void erase() noexcept {
        x_ = NAN;
        y_ = NAN;
        z_ = NAN;
    }
};

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
    // bool  intersect(const Interval &interval) const {
    //     if (interval.is_nul() || this->is_nul())
    //         return false;

    // }

    void erase() noexcept {
        p_max_.erase();
        p_min_.erase();
    }
};

class Polygon {
private:
    std::vector<Point> vertices_;

public:
    Polygon() = default;
    explicit Polygon(const std::vector<Point> &points) {
        if (points.size() > 6)
            throw std::invalid_argument("Polygon can have max 6 vertices");

        vertices_ = points;
    }

    void print() const {
        std::cout << BLUE << "polygon " << CEAN << "{\n" << RESET;
        for (std::size_t i = 0; i < vertices_.size(); ++i) {
            std::cout << "   ";
            vertices_[i].print();
        }
        std::cout << CEAN << '}' << RESET << std::endl;
    }

    bool valid() const {
        std::size_t vsz = vertices_.size();

        if (vsz == 0)
            return false;

        for (size_t i = 0; i < vsz; i++) {
            if (!vertices_[i].valid())
                return false;
        }

        return true;
    }

    bool  contains(const Point &p) const;
    bool  contains(const Vector &OP) const; // OP = радиус-вектор точки p
    bool  contains(const Line &l) const;

    bool  complanar(const Polygon &pol) const;
    bool  orthogonal(const Polygon &pol) const;
    bool  equal(const Polygon &pol) const;

    float distance(const Point &p) const;

    Line  intersect(const Polygon &pol) const;

    void  erase() noexcept {
        std::size_t vsz = vertices_.size();

        for (std::size_t i = 0; i < vsz; ++i) {
            if (vertices_[i].valid())
                vertices_[i].erase();
        }
    }
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

#endif
