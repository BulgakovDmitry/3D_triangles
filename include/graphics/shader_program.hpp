#ifndef INCLUDE_SHADER_PROGRAM_HPP
#define INCLUDE_SHADER_PROGRAM_HPP

#define GLFW_INCLUDE_NONE
#include "shaders.hpp"
#include "utils.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <utility>

namespace triangle {

struct Shader_program { // TODO similar errors as in mesh
    GLuint vertex_shader_{0};
    GLuint fragment_shader_{0};
    GLuint shader_program_{0};

    Shader_program() = default;
    Shader_program(const Shader_program &) = delete;
    Shader_program &operator=(const Shader_program &) = delete;
    Shader_program(Shader_program &&other) = default;
    Shader_program &operator=(Shader_program &&other) = default;
    ~Shader_program() { shutdown(); }

    void shutdown() noexcept { // TODO mv to private
        if (shader_program_)
            glDeleteProgram(shader_program_);
        if (vertex_shader_)
            glDeleteShader(vertex_shader_);
        if (fragment_shader_)
            glDeleteShader(fragment_shader_);
        shader_program_ = vertex_shader_ = fragment_shader_ = 0;
    }

    bool init() {
        vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader_, 1, &vertex_shader_source, NULL);
        glCompileShader(vertex_shader_);
        if (!check_shader_compile_status(vertex_shader_)) {
            return false;
        }

        fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader_, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader_);
        if (!check_shader_compile_status(fragment_shader_)) {
            return false;
        }

        shader_program_ = glCreateProgram();
        glAttachShader(shader_program_, vertex_shader_);
        glAttachShader(shader_program_, fragment_shader_);
        glLinkProgram(shader_program_);
        if (!check_program_link_status(shader_program_)) {
            return false;
        }

        glDeleteShader(vertex_shader_);
        glDeleteShader(fragment_shader_);
        return true;
    }

    const GLuint &get_shader_program() const noexcept { return shader_program_; }
};

} // namespace triangle

#endif // INCLUDE_SHADER_PROGRAM_HPP