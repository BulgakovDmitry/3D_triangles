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
    struct Shader {
        GLuint id_{0};
        Shader() = default;

        Shader(GLenum type, const char *source) {
            id_ = glCreateShader(type);
            if (!id_) {
                throw std::runtime_error("glCreateShader failed");
            }

            glShaderSource(id_, 1, &source, nullptr);
            glCompileShader(id_);

            if (!check_shader_compile_status(id_)) {
                glDeleteShader(id_);
                id_ = 0;
                throw std::runtime_error("error in check_shader_compile_status");
            }
        }

        Shader(const Shader &) = delete;
        Shader &operator=(const Shader &) = delete;

        Shader(Shader &&other) noexcept : id_{other.id_} {
            other.id_ = 0;
        }

        Shader &operator=(Shader &&other) noexcept {
            if (this != &other) {
                reset();
                id_ = other.id_;
                other.id_ = 0;
            }
            return *this;
        }

        ~Shader() { reset(); }

        void reset() noexcept {
            if (id_) {
                glDeleteShader(id_);
                id_ = 0;
            }
        }

        GLuint get() const noexcept { return id_; }
    };

    Shader vertex_shader_;
    Shader fragment_shader_;
    GLuint shader_program_{0};

  public:
    Shader_program() 
    try
        : vertex_shader_{GL_VERTEX_SHADER, vertex_shader_source}
        , fragment_shader_{GL_FRAGMENT_SHADER, fragment_shader_source} 
    {
        shader_program_ = glCreateProgram();
        if (!shader_program_) {
            throw std::runtime_error("glCreateProgram failed");
        }

        glAttachShader(shader_program_, vertex_shader_.get());
        glAttachShader(shader_program_, fragment_shader_.get());
        glLinkProgram(shader_program_);

        if (!check_program_link_status(shader_program_)) {
            throw std::runtime_error("error in check_program_link_status");
        }
    } catch (...) {
        if (shader_program_) {
            glDeleteProgram(shader_program_);
            shader_program_ = 0;
        }
        throw;
    }

    Shader_program(const Shader_program &) = delete;
    Shader_program &operator=(const Shader_program &) = delete;

    Shader_program(Shader_program &&other) noexcept
        : vertex_shader_{std::move(other.vertex_shader_)}
        , fragment_shader_{std::move(other.fragment_shader_)}
        , shader_program_{other.shader_program_} {
        other.shader_program_ = 0;
    }

    Shader_program &operator=(Shader_program &&other) noexcept {
        if (this != &other) {
            if (shader_program_) {
                glDeleteProgram(shader_program_);
            }

            vertex_shader_   = std::move(other.vertex_shader_);
            fragment_shader_ = std::move(other.fragment_shader_);
            shader_program_  = other.shader_program_;
            other.shader_program_ = 0;
        }
        return *this;
    }

    ~Shader_program() {
        if (shader_program_) {
            glDeleteProgram(shader_program_);
        }
    }

    const GLuint &get_shader_program() const noexcept { return shader_program_; }
};

} // namespace triangle

#endif // INCLUDE_SHADER_PROGRAM_HPP
