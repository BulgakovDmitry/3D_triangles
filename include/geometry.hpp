#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cmath>
#include <iostream>

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

    float get_x() const noexcept;
    float get_y() const noexcept;
    float get_z() const noexcept;

    void  print() const;

    bool  valid() const;

    bool  is_nul() const noexcept;

    bool  collinear(const Vector &v) const;
    bool  orthogonal(const Vector &v) const;

    void  erase() noexcept;
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

float  scalar_product(const Vector &v1, const Vector &v2);
Vector vector_product(const Vector &v1, const Vector &v2);

#endif
