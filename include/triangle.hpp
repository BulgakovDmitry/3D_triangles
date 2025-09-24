#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP
#include "geometry.hpp"

class Triangle {
    Point vertices_[3];

public:
    Triangle(const Point &point_0, const Point &point_1, const Point &point_2);

    bool intersect(const Triangle &triangle) const;

    const Point (&get_vertices() const)[3];

private:
    bool check_scalar_productions(Vector normal, Vector vectors[3]) const;
};

Triangle::Triangle(const Point &point_0, const Point &point_1, const Point &point_2)
    : vertices_{point_0, point_1, point_2} {}

const Point (&Triangle::get_vertices() const)[3] { return vertices_; }

bool Triangle::intersect(const Triangle &triangle) const {
    auto   vertices_2     = triangle.get_vertices();

    Vector fst_vectors[3] = {
        {vertices_[0], vertices_[1]}, {vertices_[1], vertices_[2]}, {vertices_[2], vertices_[0]}};

    Vector scd_vectors[3]   = {{vertices_2[0], vertices_2[1]},
                               {vertices_2[1], vertices_2[2]},
                               {vertices_2[2], vertices_2[0]}};

    auto   normal_fst_trngl = vector_product(fst_vectors[0], fst_vectors[1]);

    if (!check_scalar_productions(normal_fst_trngl, scd_vectors))
        return false;

    auto normal_scd_trngl = vector_product(scd_vectors[0], scd_vectors[1]);

    if (!check_scalar_productions(normal_scd_trngl, fst_vectors))
        return false;

    return true;
}

bool Triangle::check_scalar_productions(Vector normal, Vector vectors[3]) const {
    bool   all_negatives = true, all_pozitives = true;

    for (size_t i = 0; i < 3; i++) {
        if (scalar_product(normal, vectors[i]) >= 0)
            all_negatives = false;
        else
            all_pozitives = false;
    }

    if (all_negatives || all_pozitives)
        return false;

    return true;
}

#endif
