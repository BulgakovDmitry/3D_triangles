#ifndef INCLUDE_PRIMITIVES_POLYGON_HPP
#define INCLUDE_PRIMITIVES_POLYGON_HPP

#include <stdexcept>
#include <iostream>
#include <vector>
#include "../common/cmp.hpp"
#include "../common/colors.hpp"
#include "point.hpp"
#include "vector.hpp"
#include "line.hpp"

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
    //bool  equal(const Polygon &pol) const; //TODO operator==

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

#endif // INCLUDE_PRIMITIVES_POLYGON_HPP