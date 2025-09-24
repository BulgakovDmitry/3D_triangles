
#include "geometry.hpp"
#include "triangle.hpp"

int main() {
    Triangle tr1({0, 0, 2}, {0, 1, 0}, {1, 0, 0});
    Triangle tr2({0, 0, 1}, {0, 2, 0}, {2, 0, 0});

    // std::cout << "result of intersect: " << tr1.intersect(tr2) << '\n';
}
