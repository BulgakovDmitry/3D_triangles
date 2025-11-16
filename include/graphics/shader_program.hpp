#ifndef INCLUDE_SHADER_PROGRAM_HPP
#define INCLUDE_SHADER_PROGRAM_HPP

#define GLFW_INCLUDE_NONE
#include "shaders.hpp"
#include "utils.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace triangle {

struct Shader_program {
  private:
    GLuint vertex_shader_{0};
    GLuint fragment_shader_{0};
    GLuint shader_program_{0};

  public:
    Shader_program() {
    try {
        vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader_, 1, &vertex_shader_source, NULL);
        glCompileShader(vertex_shader_);
        if (!check_shader_compile_status(vertex_shader_)) {
            throw std::runtime_error("error in check_shader_compile_status (vertex shader)");
        }

        fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader_, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader_);
        if (!check_shader_compile_status(fragment_shader_)) {
            throw std::runtime_error("error in check_shader_compile_status (fragment shader)");
        }

        shader_program_ = glCreateProgram();
        glAttachShader(shader_program_, vertex_shader_);
        glAttachShader(shader_program_, fragment_shader_);
        glLinkProgram(shader_program_);
        if (!check_program_link_status(shader_program_)) {
            throw std::runtime_error("error in check_program_link_status");
        }

        glDeleteShader(vertex_shader_);
        glDeleteShader(fragment_shader_);
    } catch (...) {
        shutdown();
        throw;
    }
    }

    Shader_program(const Shader_program &) = delete;
    Shader_program &operator=(const Shader_program &) = delete;
    Shader_program(Shader_program &&other) = default;
    Shader_program &operator=(Shader_program &&other) = default;
    ~Shader_program() { shutdown(); }

  private:
    void shutdown() noexcept {
        if (shader_program_)
            glDeleteProgram(shader_program_);
        if (vertex_shader_)
            glDeleteShader(vertex_shader_);
        if (fragment_shader_)
            glDeleteShader(fragment_shader_);
        shader_program_ = vertex_shader_ = fragment_shader_ = 0;
    }

  public:
    const GLuint &get_shader_program() const noexcept { return shader_program_; }
};

} // namespace triangle

#endif // INCLUDE_SHADER_PROGRAM_HPP
