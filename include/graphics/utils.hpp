#ifndef INCLUDE_GRAPHICS_UTILS_HPP
#define INCLUDE_GRAPHICS_UTILS_HPP

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>

#include "primitives/point.hpp"
#include "primitives/triangle.hpp"

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

static std::vector<float> get_vector_all_vertices(std::vector<Triangle<float>> &triangles) {
    std::vector<float> all_vertices;

    for (const auto &triangle : triangles) {
        auto vertices = triangle.get_vertices();

        for (const auto &vertex : vertices) {
            all_vertices.push_back(vertex.x_);
            all_vertices.push_back(vertex.y_);
            all_vertices.push_back(vertex.z_);
        }
    }

    return all_vertices;
}

} // namespace triangle

#endif // INCLUDE_GRAPHICS_UTILS_HPP
