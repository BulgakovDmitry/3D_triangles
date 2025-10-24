#ifndef GRAPHICS_DRIVER_HPP
#define GRAPHICS_DRIVER_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstddef>
#include <glad/glad.h>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "graphics/shaders.hpp"
#include "graphics/utils.hpp"
#include "primitives/triangle.hpp"

namespace triangle {

class Graphics_driver {
  private:
    GLFWwindow *window_;

    GLuint VAO_;
    GLuint VBO_;

    GLuint vertex_shader_;
    GLuint fragment_shader_;
    GLuint shader_program_;

  public:
    Graphics_driver() = default;
    ~Graphics_driver() { /*shutdown();*/ }

    Graphics_driver(const Graphics_driver &) = delete;
    Graphics_driver &operator=(const Graphics_driver &) = delete;
    Graphics_driver(Graphics_driver &&) = default;            // TODO
    Graphics_driver &operator=(Graphics_driver &&) = default; // TODO

    const GLFWwindow *get_window() const noexcept { return window_; }
    GLFWwindow *get_window() noexcept { return window_; }

    const GLuint &get_VAO() const noexcept { return VAO_; }
    const GLuint &get_VBO() const noexcept { return VBO_; }
    const GLuint &get_vertex_shader_() const noexcept { return vertex_shader_; }
    const GLuint &get_fragment_shader_() const noexcept { return fragment_shader_; }
    const GLuint &get_shader_program_() const noexcept { return shader_program_; }

    bool init_graphics(std::vector<float> &all_vertices);
};

inline void graphics_driver(std::vector<Triangle<float>> &triangles,
                            std::unordered_set<std::size_t> &intersecting_triangles) {
    auto all_vertices = get_vector_all_vertices(triangles);

    Graphics_driver gv;

    if (!gv.init_graphics(all_vertices))
        return;

    int time_loc = glGetUniformLocation(gv.get_shader_program_(), "u_time");
    if (time_loc == -1) {
        std::cerr << "ERROR: Uniform 'u_time' not found in shader_program" << std::endl;
        return;
    }

    while (!glfwWindowShouldClose(gv.get_window())) {
        glfwPollEvents();

        auto time = static_cast<float>(glfwGetTime());

        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(gv.get_shader_program_());
        glUniform1f(time_loc, time);
        glBindVertexArray(gv.get_VAO());
        glDrawArrays(GL_TRIANGLES, 0, all_vertices.size() / 3);

        glfwSwapBuffers(gv.get_window());
    }

    glDeleteVertexArrays(1, &gv.get_VAO());
    glDeleteBuffers(1, &gv.get_VBO());
    glDeleteProgram(gv.get_shader_program_());

    glfwDestroyWindow(gv.get_window());
    glfwTerminate();
}

bool Graphics_driver::init_graphics(std::vector<float> &all_vertices) {
    if (all_vertices.empty()) {
        std::cerr << "[init_graphics] empty vertex array\n";
        return false;
    }

    if (!glfwInit()) {
        std::cerr << "Failed to initialize glfw" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window_ = glfwCreateWindow(1000, 800, "OpenGL 4.6 Window", nullptr, nullptr);
    if (window_ == nullptr) {
        std::cout << "Create window failed" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    check_GL_error("glGenBuffers");

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, all_vertices.size() * sizeof(float), all_vertices.data(),
                 GL_STATIC_DRAW);
    check_GL_error("glBufferData");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    check_GL_error("glVertexAttribPointer");

    vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader_);

    if (!check_shader_compile_status(vertex_shader_)) {
        glfwDestroyWindow(window_);
        glfwTerminate();
        return false;
    }

    fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader_);

    if (!check_shader_compile_status(fragment_shader_)) {
        glDeleteShader(vertex_shader_);
        glfwDestroyWindow(window_);
        glfwTerminate();
        return false;
    }

    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vertex_shader_);
    glAttachShader(shader_program_, fragment_shader_);
    glLinkProgram(shader_program_);

    if (!check_program_link_status(shader_program_)) {
        glDeleteShader(vertex_shader_);
        glDeleteShader(fragment_shader_);
        glfwDestroyWindow(window_);
        glfwTerminate();
        return false;
    }

    glDeleteShader(vertex_shader_);
    glDeleteShader(fragment_shader_);

    return true;
}

} // namespace triangle

#endif
