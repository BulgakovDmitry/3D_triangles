#ifndef POINT_TO_SEGMENT_HPP
#define POINT_TO_SEGMENT_HPP

#include "common/cmp.hpp"
#include "primitives/point.hpp"
#include "primitives/vector.hpp"

namespace triangle {

template <std::floating_point T>
bool is_point_on_segment(const Point<T> &A, const Point<T> &B,
                         const Point<T> &P) {
    Vector<T> AP{A, P};
    Vector<T> AB{A, B};

    // Checking collinearity using vector product
    Vector<T> cross = vector_product(AP, AB);
    if (!cmp::is_zero(cross.x_) || !cmp::is_zero(cross.y_) ||
        !cmp::is_zero(cross.z_))
        return false;

    // Check if a point is between A and B via the dot product
    T dot = scalar_product(AP, AB);
    if (cmp::negative(dot) || dot > scalar_product(AB, AB) + cmp::precision<T>::epsilon)
        return false;

    return true;
}

} // namespace triangle

#endif
