#include "geometry.hpp"

class Triangle {
    Point   vertices_[3];
    Polygon polygon_;

public:
    Triangle(const Point &point_0, const Point &point_1, const Point &point_2);

    const Polygon(&get_polygon() const);

    bool intersect(const Triangle &triangle) const;

    const Point (&get_vertices() const)[3];
private:
    bool check_relative_positions(const Triangle &triangle) const;
};

Triangle::Triangle(const Point &point_0, const Point &point_1, const Point &point_2)
    : vertices_{point_0, point_1, point_2} {}

const Polygon(&Triangle::get_polygon() const) { return polygon_; }

const Point (&Triangle::get_vertices() const)[3] { return vertices_; }

bool Triangle::intersect(const Triangle &triangle) const {
    // get polygons
    Polygon second_polygon = triangle.get_polygon();

    // check the relative position of the vertices of one triangle relative to another
    if (!Triangle::check_relative_positions(triangle))
        return false;

    // checking that Polygons are not parallel
    if (polygon_.complanar(second_polygon))
        return false;

    // get intersect line
    Line     intersect_line  = polygon_.intersect(second_polygon);

    // get projections to intersect line (intervals)
    Interval first_interval  = Interval(intersect_line, {vertices_[0], vertices_[1], vertices_[2]});

    auto     second_vertices = triangle.get_vertices();
    Interval second_interval =
        Interval(intersect_line, {second_vertices[0], second_vertices[1], second_vertices[2]});

    // compare intervals
    return first_interval.intersect(second_interval);
}

bool Triangle::check_relative_positions(const Triangle &triangle) const {
    auto  polygon_2  = triangle.get_polygon();
    auto  vertices_2 = triangle.get_vertices();

    float distance_2[3];
    bool  all_positive_2 = true, all_negative_2 = true;

    for (int i = 0; i < 3; i++) {
        distance_2[i] = polygon_.distance(vertices_2[i]);

        if (distance_2[i] >= 0)
            all_negative_2 = false;

        if (distance_2[i] <= 0)
            all_positive_2 = false;
    }

    if (all_positive_2 || all_negative_2)
        return false;

    float distance_1[3];
    bool  all_positive_1 = true, all_negative_1 = true;

    for (int i = 0; i < 3; i++) {
        distance_1[i] = polygon_2.distance(vertices_[i]);

        if (distance_1[i] >= 0)
            all_negative_1 = false;

        if (distance_1[i] <= 0)
            all_positive_1 = false;
    }

    if (all_positive_1 || all_negative_1)
        return false;

    return true;
}
