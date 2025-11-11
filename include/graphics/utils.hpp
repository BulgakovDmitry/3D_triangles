#ifndef INCLUDE_GRAPHICS_UTILS_HPP
#define INCLUDE_GRAPHICS_UTILS_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "primitives/point.hpp"
#include "primitives/triangle.hpp"
#include "primitives/vector.hpp"

namespace triangle {

static bool check_shader_compile_status(unsigned int shader) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        std::cerr << "ERROR: shaider compilation failed\n" << info_log << std::endl;
        return false;
    }
    return true;
}

static bool check_program_link_status(unsigned int program) {
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program, 512, nullptr, info_log);
        std::cerr << "Shader program linking failed:\n" << info_log << std::endl;
        return false;
    }
    return true;
}

static void check_GL_error(const std::string &context) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error in " << context << ": " << error << std::endl;
    }
}

static void add_vertices(std::vector<float> &vertices_vector, std::array<Point<float>, 3> &points) {
    for (const auto &point : points) {
        vertices_vector.push_back(point.x_);
        vertices_vector.push_back(point.y_);
        vertices_vector.push_back(point.z_);
    }
}
static void add_normal(std::vector<float> &vertices_vector, std::array<Point<float>, 3> &points) {
    Vector ab{points[1], points[0]};
    Vector ac{points[2], points[0]};

    Vector normal = vector_product(ab, ac);

    vertices_vector.push_back(normal.x_);
    vertices_vector.push_back(normal.y_);
    vertices_vector.push_back(normal.z_);
}

inline std::pair<std::vector<float>, std::vector<float>>
get_vector_all_vertices(std::vector<Triangle<float>> &triangles,
                        std::unordered_set<std::size_t> &intersecting_triangles) {
    std::vector<float> blue_vertices;
    std::vector<float> red_vertices;

    for (const auto &triangle : triangles) {
        auto vertices = triangle.get_vertices();
        if (intersecting_triangles.contains(triangle.get_id())) {
            add_vertices(red_vertices, vertices);
            add_normal(red_vertices, vertices);
        } else {
            add_vertices(blue_vertices, vertices);
            add_normal(red_vertices, vertices);
        }
    }

    return {blue_vertices, red_vertices};
}

} // namespace triangle

#endif // INCLUDE_GRAPHICS_UTILS_HPP
