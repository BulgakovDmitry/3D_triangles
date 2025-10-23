#ifndef GRAPHICS_DRIVER_HPP
#define GRAPHICS_DRIVER_HPP

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>

#include "driver.hpp"
#include "primitives/point.hpp"

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

static std::vector<float> get_vector_from_vertices(std::array<Point<float>, 3> vertices) {
    std::vector<float> vertex_data;
    vertex_data.reserve(vertices.size() * 3);

    for (const auto &point : vertices) {
        vertex_data.push_back(point.x_);
        vertex_data.push_back(point.y_);
        vertex_data.push_back(point.z_);
    }

    return vertex_data;
}

inline void graphics_driver() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(1000, 800, "OpenGL 4.6 Window", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Create window failed" << std::endl;
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);

    auto triangles = get_input_data<float>();

    auto intersecting_triangles = driver<float>(triangles);

    auto vertices = get_vector_from_vertices(triangles[0].get_vertices());

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    check_GL_error("glGenBuffers");

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    check_GL_error("glBufferData");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    check_GL_error("glVertexAttribPointer");

    const char *vertex_shader_source = R"(
        #version 460 core
        layout (location = 0) in vec3 aPos;

        uniform float uTime;

        void main() {
            float angle = uTime;
            float c = cos(angle);
            float s = sin(angle);

            mat3 rotation = mat3 (
                c, 0.0, -s,
                0.0, 1.0, 0.0,
                s, 0.0, c
            );

            vec3 rotated = rotation * aPos;
            gl_Position = vec4(rotated, 1.0);
        }
    )";

    const char *fragment_shader_source = R"(
        #version 460 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
    )";

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    if (!check_shader_compile_status(vertex_shader)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    if (!check_shader_compile_status(fragment_shader)) {
        glDeleteShader(vertex_shader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    if (!check_program_link_status(shader_program)) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int time_loc = glGetUniformLocation(shader_program, "uTime");
    if (time_loc == -1) {
        std::cerr << "ERROR: Uniform 'uTime' not found in shader_program" << std::endl;
        return;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        auto time = static_cast<float>(glfwGetTime());

        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_program);
        glUniform1f(time_loc, time);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);

    glfwDestroyWindow(window);
    glfwTerminate();
}

} // namespace triangle

#endif
