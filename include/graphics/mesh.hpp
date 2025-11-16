#ifndef INCLUDE_MESH_HPP
#define INCLUDE_MESH_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <utility>
#include <vector>

#include "utils.hpp"

namespace triangle {

struct Mesh {
  private:
    GLuint vao;
    GLuint vbo;

  public:
    Mesh(const std::vector<float> &positions, GLint attrib_location = 0,
         GLint attrib_location_normal = 1, GLsizei components = 3, GLsizei stride = 0) : vao(0)
        , vbo(0) {
        try {
            glGenVertexArrays(1, &vao);
            verify_gl("Mesh ctor: glGenVertexArrays");

            glGenBuffers(1, &vbo);
            verify_gl("Mesh ctor: glGenBuffers");

            glBindVertexArray(vao);
            verify_gl("Mesh ctor: glBindVertexArray");

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            verify_gl("Mesh ctor: glBindBuffer(GL_ARRAY_BUFFER)");

            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(positions.size() * sizeof(float)),
                        positions.data(), GL_STATIC_DRAW);
            verify_gl("Mesh ctor: glBufferData");

            if (stride == 0)
                stride = 2 * components * sizeof(float); // 2 one for vertex and one for normal

            glVertexAttribPointer(attrib_location, components, GL_FLOAT, GL_FALSE, stride, (void *)0);
            verify_gl("Mesh ctor: glVertexAttribPointer(0)");

            glEnableVertexAttribArray(attrib_location);
            verify_gl("Mesh ctor: glEnableVertexAttribArray(position)");

            glVertexAttribPointer(attrib_location_normal, components, GL_FLOAT, GL_FALSE, stride,
                                (void *)(3 * sizeof(float)));
            verify_gl("Mesh ctor: glVertexAttribPointer(3)");
            
            glEnableVertexAttribArray(attrib_location_normal);
            verify_gl("Mesh ctor: glEnableVertexAttribArray(normal)");

            glBindVertexArray(0);
            verify_gl("Mesh ctor: glBindBuffer");
        } catch (...) {
            reset();
            throw;
        }
    }

    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;
    Mesh &operator=(Mesh &&other) noexcept {
        if (this != &other) {
            reset();
            vao = std::exchange(other.vao, 0);
            vbo = std::exchange(other.vbo, 0);
        }
        return *this;
    }
    ~Mesh() { reset(); }

    void reset() noexcept {
        if (vbo != 0) {
            glDeleteBuffers(1, &vbo);
            vbo = 0;
        }
        if (vao != 0) {
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }
    }

    void bind() const noexcept { glBindVertexArray(vao); }
};

} // namespace triangle

#endif // INCLUDE_MESH_HPP
