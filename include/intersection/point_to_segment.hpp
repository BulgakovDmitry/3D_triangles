#ifndef POINT_TO_SEGMENT_HPP
#define POINT_TO_SEGMENT_HPP

#include "common/cmp.hpp"
#include "primitives/point.hpp"
#include "primitives/vector.hpp"

inline bool is_point_on_segment(const Point &A, const Point &B, const Point &P) {
    Vector AP{A, P};
    Vector BP{P, B};
    Vector AB{A, B};

    // Checking collinearity using vector product
    Vector cross = vector_product(AP, AB);
    if (!cmp::is_zero(cross.get_x()) || !cmp::is_zero(cross.get_y()) ||
        !cmp::is_zero(cross.get_z()))
        return false;

    // Check if a point is between A and B via the dot product
    double dot = scalar_product(AP, AB);
    if (cmp::negative(dot) || dot > scalar_product(AB, AB) + cmp::float_eps)
        return false;

    return true;
}

#endif
