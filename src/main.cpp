#include "driver.hpp"

using namespace triangle;

int main() {
    auto triangles = get_input_data<float>();

    auto intersecting_triangles = driver<float>(triangles);

    print_numbers_of_intersecting_triangles(intersecting_triangles);

    return 0;
}
