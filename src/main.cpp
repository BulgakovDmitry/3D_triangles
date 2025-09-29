#include "driver.hpp"
#include "primitives/line.hpp"
#include "primitives/point.hpp"
#include "primitives/vector.hpp"
#include "triangle.hpp"
#include <iostream>

using namespace triangle;

int main() {
    // Triangle tr1(Point(0, 0, 2), Point(0, 1, 0), Point(1, 0, 0));
    // Triangle tr2(Point(0, 0, 1), Point(0, 2, 0), Point(2, 0, 0));
    // Triangle tr3(Point(0, 0, 3), Point(0, 2, 0), Point(2, 0, 0));

    // std::cout << "result of intersect: " << tr1.intersect(tr2) << '\n';
    // std::cout << "result of intersect: " << tr1.intersect(tr3) << '\n';

    driver();
}
