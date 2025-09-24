#include "geometry.hpp"
#include <iostream>

int main() {
    Point  a(1, 1, 1);
    Point  b(2, 1, 1);

    Point  c(3, 2, 1);
    Point  d(3, 6, 1);

    Vector v1(a, b);
    Vector v2(c, d);

    v1.print();
    v2.print();

    std::cout << "v1 || v2 = " << v1.collinear(v2) << std::endl;
    std::cout << "v1 or v2 = " << v1.orthogonal(v2) << std::endl;

    v2.erase();
    std::cout << "v2.valid() = " << v2.valid() << std::endl;
    std::cout << "c.valid() = " << c.valid() << std::endl;

    Vector v(3, 4, 0);
    std::cout << "len v = " << v.abs() << std::endl;

    Polygon pol({a, b, c});
    pol.print();

    Vector v3(1, 1, 0);
    Vector v4(2, 0, 0);
    Vector v5(0, 5, 1);

    (v3.projection(v4)).print();
    (v4.projection(v5)).print();

    //-----------------------------
    Point    p1(1, 0, 0);
    Point    p2(2, -200, 0);
    // Point p3(4, 0, 0);
    // Point p4(7, 0, 0);

    Point    p0(0, 1, 0);
    Vector   a0(1, 0, 0);

    Line     l0(p0, a0);

    Interval inter(l0, {p1, p2});
    inter.get_p_max().print();
    inter.get_p_min().print();

    return 0;
}
