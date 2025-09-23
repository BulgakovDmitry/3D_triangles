#include "geometry.hpp"

class Triangle {
    Point triangle_vertices_[3];

public:
    Triangle(const Point &point_0, const Point &point_1, const Point &point_2);

    Polygon get_polygon() const;

    bool intersect(const Triangle &triangle) const;

    const Point (&get_vertices() const)[3];
};

Triangle::Triangle(const Point &point_0, const Point &point_1, const Point &point_2)
    : triangle_vertices_{point_0, point_1, point_2} {}

Polygon Triangle::get_polygon() const {
    return Polygon({triangle_vertices_[0], triangle_vertices_[1], triangle_vertices_[2]});
}

const Point (&Triangle::get_vertices() const)[3] { return triangle_vertices_; }

bool Triangle::intersect(const Triangle &triangle) const {
    // get polygons
    Polygon first_polygon  = get_polygon();
    Polygon second_polygon = triangle.get_polygon();

    // checking that Polygons are not parallel
    if (first_polygon.complanar(second_polygon))
        return false;

    // get intersect line
    Line     intersect_line = first_polygon.intersect(second_polygon);

    // get projections to intersect line (intervals)
    Interval first_interval = Interval(
        intersect_line, {triangle_vertices_[0], triangle_vertices_[1], triangle_vertices_[2]});

    auto     second_vertices = triangle.get_vertices();
    Interval second_interval =
        Interval(intersect_line, {second_vertices[0], second_vertices[1], second_vertices[2]});

    // compare intervals
    return first_interval.intersect(second_interval);
}
