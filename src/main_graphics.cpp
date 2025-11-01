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

    auto [blue_vertices, red_vertices] = get_vector_all_vertices(triangles, intersect_triangles_us);

    Graphics_driver gd(std::move(blue_vertices), std::move(red_vertices));
    gd.graphics_run();

    return 0;
}
