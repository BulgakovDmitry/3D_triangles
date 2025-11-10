#ifndef INCLUDE_MESH_HPP
#define INCLUDE_MESH_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <cstddef>
#include <utility>
#include <vector>

namespace triangle {

struct Mesh {      // FIXME create no default ctor (mv from init)
    GLuint vao{0}; // TODO it is private
    GLuint vbo{0}; // TODO it is private

    Mesh() = default;
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

    bool init_from_positions(const std::vector<float> &positions,
                             GLint attrib_location = 0, // TODO delete
                             GLsizei components = 3, GLsizei stride = 0) {
        if (positions.empty()) {
            reset();
            return true;
        }

        if (vao == 0)
            glGenVertexArrays(1, &vao);
        if (vbo == 0)
            glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(positions.size() * sizeof(float)),
                     positions.data(), GL_STATIC_DRAW);

        if (stride == 0)
            stride = components * sizeof(float);

        glVertexAttribPointer(attrib_location, components, GL_FLOAT, GL_FALSE, stride, (void *)0);
        glEnableVertexAttribArray(attrib_location);

        glBindVertexArray(0);
        return true;
    }

    void bind() const noexcept { glBindVertexArray(vao); }
};

} // namespace triangle

#endif // INCLUDE_MESH_HPP