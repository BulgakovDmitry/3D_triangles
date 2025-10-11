#ifndef POINT_TO_SEGMENT_HPP
#define POINT_TO_SEGMENT_HPP

#include "common/cmp.hpp"
#include "primitives/point.hpp"
#include "primitives/vector.hpp"

template <std::floating_point T>
bool is_point_on_segment(const triangle::Point<T> &A, const triangle::Point<T> &B,
                         const triangle::Point<T> &P) {
    triangle::Vector<T> AP{A, P};
    triangle::Vector<T> AB{A, B};

    // Checking collinearity using vector product
    triangle::Vector<T> cross = vector_product(AP, AB);
    if (!cmp::is_zero(cross.get_x()) || !cmp::is_zero(cross.get_y()) ||
        !cmp::is_zero(cross.get_z()))
        return false;

    // Check if a point is between A and B via the dot product
    T dot = scalar_product(AP, AB);
    if (cmp::negative(dot) || dot > scalar_product(AB, AB) + cmp::float_eps)
        return false;

    return true;
}

#endif
