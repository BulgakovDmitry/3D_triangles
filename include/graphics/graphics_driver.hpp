#ifndef GRAPHICS_DRIVER_HPP
#define GRAPHICS_DRIVER_HPP

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstddef>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "graphics/shaders.hpp"
#include "graphics/utils.hpp"
#include "primitives/triangle.hpp"

namespace triangle {

struct graphics {
    GLFWwindow *window;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int vertex_shader;
    unsigned int fragment_shader;
    unsigned int shader_program;
};

static bool init_graphics(graphics &driver_struct, std::vector<float> &all_vertices);

inline void graphics_driver(std::vector<Triangle<float>> &triangles,
                            std::unordered_set<std::size_t> &intersecting_triangles) {
    auto all_vertices = get_vector_all_vertices(triangles);

    graphics driver_struct;

    if (!init_graphics(driver_struct, all_vertices))
        return;

    int time_loc = glGetUniformLocation(driver_struct.shader_program, "u_time");
    if (time_loc == -1) {
        std::cerr << "ERROR: Uniform 'u_time' not found in shader_program" << std::endl;
        return;
    }

    while (!glfwWindowShouldClose(driver_struct.window)) {
        glfwPollEvents();

        auto time = static_cast<float>(glfwGetTime());

        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(driver_struct.shader_program);
        glUniform1f(time_loc, time);
        glBindVertexArray(driver_struct.VAO);
        glDrawArrays(GL_TRIANGLES, 0, all_vertices.size() / 3);

        glfwSwapBuffers(driver_struct.window);
    }

    glDeleteVertexArrays(1, &driver_struct.VAO);
    glDeleteBuffers(1, &driver_struct.VBO);
    glDeleteProgram(driver_struct.shader_program);

    glfwDestroyWindow(driver_struct.window);
    glfwTerminate();
}

static bool init_graphics(graphics &driver_struct, std::vector<float> &all_vertices) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    driver_struct.window = glfwCreateWindow(1000, 800, "OpenGL 4.6 Window", nullptr, nullptr);
    if (driver_struct.window == nullptr) {
        std::cout << "Create window failed" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(driver_struct.window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &driver_struct.VAO);
    glGenBuffers(1, &driver_struct.VBO);
    check_GL_error("glGenBuffers");

    glBindVertexArray(driver_struct.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, driver_struct.VBO);
    glBufferData(GL_ARRAY_BUFFER, all_vertices.size() * sizeof(float), all_vertices.data(),
                 GL_STATIC_DRAW);
    check_GL_error("glBufferData");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    check_GL_error("glVertexAttribPointer");

    driver_struct.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(driver_struct.vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(driver_struct.vertex_shader);

    if (!check_shader_compile_status(driver_struct.vertex_shader)) {
        glfwDestroyWindow(driver_struct.window);
        glfwTerminate();
        return false;
    }

    driver_struct.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(driver_struct.fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(driver_struct.fragment_shader);

    if (!check_shader_compile_status(driver_struct.fragment_shader)) {
        glDeleteShader(driver_struct.vertex_shader);
        glfwDestroyWindow(driver_struct.window);
        glfwTerminate();
        return false;
    }

    driver_struct.shader_program = glCreateProgram();
    glAttachShader(driver_struct.shader_program, driver_struct.vertex_shader);
    glAttachShader(driver_struct.shader_program, driver_struct.fragment_shader);
    glLinkProgram(driver_struct.shader_program);

    if (!check_program_link_status(driver_struct.shader_program)) {
        glDeleteShader(driver_struct.vertex_shader);
        glDeleteShader(driver_struct.fragment_shader);
        glfwDestroyWindow(driver_struct.window);
        glfwTerminate();
        return false;
    }

    glDeleteShader(driver_struct.vertex_shader);
    glDeleteShader(driver_struct.fragment_shader);

    return true;
}

} // namespace triangle

#endif
