#include "geometry.hpp"

class Triangle {
    Point triangle_vertices_[3];

public:
    Triangle(Point point_0, Point point_1, Point point_2);

    Polygon get_polygon() const;

    const Point (&get_vertices() const)[3];

    bool intersect(const Triangle &triangle);
};

Triangle::Triangle(Point point_0, Point point_1, Point point_2)
    : triangle_vertices_{point_0, point_1, point_2} {}

Polygon Triangle::get_polygon() const {
    return Polygon({triangle_vertices_[0], triangle_vertices_[1], triangle_vertices_[2]});
}

const Point (&Triangle::get_vertices() const)[3] { return triangle_vertices_; }

bool Triangle::intersect(const Triangle &triangle) {
    // get polygons
    Polygon first_polygon  = get_polygon();
    Polygon second_polygon = triangle.get_polygon();

    // checking that Polygons are not parallel
    if (first_polygon.parallel(second_polygon))
        return false;

    // get intersect line
    Line     intersect_line  = first_polygon.intersect(second_polygon);

    // get projections to intersect line (intervals)
    Interval first_interval  = Interval(intersect_line, triangle_vertices_);
    Interval second_interval = Interval(intersect_line, triangle.get_vertices());

    // compare intervals
    return first_interval.intersect(second_interval);
}
