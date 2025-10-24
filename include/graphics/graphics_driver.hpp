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

    GLuint vao_blue_;
    GLuint vao_red_;
    GLuint vbo_blue_;
    GLuint vbo_red_;

    GLuint vertex_shader_;
    GLuint fragment_shader_;
    GLuint shader_program_;

  public:
    Graphics_driver() = default;
    ~Graphics_driver() { shutdown(); }

    Graphics_driver(const Graphics_driver &) = delete;
    Graphics_driver &operator=(const Graphics_driver &) = delete;
    Graphics_driver(Graphics_driver &&other) noexcept;
    Graphics_driver &operator=(Graphics_driver &&other) noexcept;

    const GLFWwindow *get_window() const noexcept { return window_; }
    GLFWwindow *get_window() noexcept { return window_; }

    const GLuint &get_vao_blue() const noexcept { return vao_blue_; }
    const GLuint &get_vao_red() const noexcept { return vao_red_; }
    const GLuint &get_vbo_blue() const noexcept { return vbo_blue_; }
    const GLuint &get_vbo_red() const noexcept { return vbo_red_; }
    const GLuint &get_vertex_shader_() const noexcept { return vertex_shader_; }
    const GLuint &get_fragment_shader_() const noexcept { return fragment_shader_; }
    const GLuint &get_shader_program_() const noexcept { return shader_program_; }

    bool init_graphics(std::vector<float> &blue_vertices, std::vector<float> &red_vertices);

    void run_loop(std::vector<float> &blue_vertices, std::vector<float> &red_vertices);

  private:
    void shutdown() noexcept;
};

inline void graphics_driver(std::vector<Triangle<float>> &triangles,
                            std::unordered_set<std::size_t> &intersecting_triangles) {
    auto [blue_vertices, red_vertices] = get_vector_all_vertices(triangles, intersecting_triangles);

    Graphics_driver gd;

    if (!gd.init_graphics(blue_vertices, red_vertices))
        return;

    gd.run_loop(blue_vertices, red_vertices);
}

inline bool Graphics_driver::init_graphics(std::vector<float> &blue_vertices, std::vector<float> &red_vertices) {
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

    glGenVertexArrays(1, &vao_blue_);
    glGenBuffers(1, &vbo_blue_);
    check_GL_error("glGenBuffers");

    glGenVertexArrays(1, &vao_red_);
    glGenBuffers(1, &vbo_red_);
    check_GL_error("glGenBuffers");

    glBindVertexArray(vao_blue_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_blue_);
    glBufferData(GL_ARRAY_BUFFER, blue_vertices.size() * sizeof(float), blue_vertices.data(),
                 GL_STATIC_DRAW);
    check_GL_error("glBufferData");

    glBindVertexArray(vao_red_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_red_);
    glBufferData(GL_ARRAY_BUFFER, red_vertices.size() * sizeof(float), red_vertices.data(),
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

inline void Graphics_driver::shutdown() noexcept {
    if (window_) {
        glfwMakeContextCurrent(window_);
    }

    if (shader_program_ != 0) {
        glUseProgram(0);
    }

    if (vbo_blue_ != 0) {
        glDeleteBuffers(1, &vbo_blue_);
        vbo_blue_ = 0;
    }

    if (vbo_red_ != 0) {
        glDeleteBuffers(1, &vbo_red_);
        vbo_red_ = 0;
    }

    if (vao_blue_ != 0) {
        glDeleteVertexArrays(1, &vao_blue_);
        vao_blue_ = 0;
    }

    if (vao_red_ != 0) {
        glDeleteVertexArrays(1, &vao_red_);
        vao_red_ = 0;
    }

    if (shader_program_ != 0) {
        glDeleteProgram(shader_program_);
        shader_program_ = 0;
    }
    if (vertex_shader_ != 0) {
        glDeleteShader(vertex_shader_);
        vertex_shader_ = 0;
    }
    if (fragment_shader_ != 0) {
        glDeleteShader(fragment_shader_);
        fragment_shader_ = 0;
    }

    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }

    glfwTerminate();
}

inline void Graphics_driver::run_loop(std::vector<float> &blue_vertices, std::vector<float> &red_vertices) {
    GLint material_color_loc = glGetUniformLocation(get_shader_program_(), "material_color");
    if (material_color_loc == -1) {
        std::cerr << "ERROR: Uniform 'material_color' not found in shader_program" << std::endl;
        return;
    }

    GLint time_loc = glGetUniformLocation(get_shader_program_(), "u_time");
    if (time_loc == -1) {
        std::cerr << "ERROR: Uniform 'u_time' not found in shader_program" << std::endl;
        return;
    }
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        auto time = static_cast<float>(glfwGetTime());

        glClearColor(0.75f, 0.85f, 0.90f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(get_shader_program_());
        glUniform1f(time_loc, time);

        if (!blue_vertices.empty()) {
            glBindVertexArray(vao_blue_);
            glUniform3f(material_color_loc, 0.30f, 0.50f, 0.60f);
            glDrawArrays(GL_TRIANGLES, 0, blue_vertices.size() / 3);
        }

        if (!red_vertices.empty()) {
            glBindVertexArray(vao_red_);
            glUniform3f(material_color_loc, 0.70f, 0.35f, 0.25f);
            glDrawArrays(GL_TRIANGLES, 0, red_vertices.size() / 3);
        }

        glfwSwapBuffers(window_);
    }
}

inline Graphics_driver::Graphics_driver(Graphics_driver &&other) noexcept
    : window_(std::exchange(other.window_, nullptr)), vao_blue_(std::exchange(other.vao_blue_, 0)),
        vao_red_(std::exchange(other.vao_red_, 0)), vbo_red_(std::exchange(other.vbo_red_, 0)),
      vbo_blue_(std::exchange(other.vbo_blue_, 0)), shader_program_(std::exchange(other.shader_program_, 0)),
      vertex_shader_(std::exchange(other.vertex_shader_, 0)),
      fragment_shader_(std::exchange(other.fragment_shader_, 0)) {}

inline Graphics_driver &Graphics_driver::operator=(Graphics_driver &&other) noexcept {
    if (this == &other)
        return *this;

    shutdown();

    window_ = std::exchange(other.window_, nullptr);
    vao_blue_ = std::exchange(other.vao_blue_, 0);
    vao_red_ = std::exchange(other.vao_red_, 0);
    vbo_blue_ = std::exchange(other.vbo_blue_, 0);
    vbo_red_ = std::exchange(other.vbo_red_, 0);
    shader_program_ = std::exchange(other.shader_program_, 0);
    vertex_shader_ = std::exchange(other.vertex_shader_, 0);
    fragment_shader_ = std::exchange(other.fragment_shader_, 0);

    return *this;
}

} // namespace triangle

#endif // GRAPHICS_DRIVER_HPP
