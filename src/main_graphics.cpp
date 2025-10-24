#include <cstddef>

#include "driver.hpp"
#include "graphics/graphics_driver.hpp"

using namespace triangle;

int main() {
    auto triangles = get_input_data<float>();
    auto intersect_triangles_s = driver<float>(triangles);

    using unordered_set = std::unordered_set<std::size_t>;

    unordered_set intersect_triangles_us(intersect_triangles_s.begin(),
                                         intersect_triangles_s.end());

    graphics_driver(triangles, intersect_triangles_us);

    return 0;
}
