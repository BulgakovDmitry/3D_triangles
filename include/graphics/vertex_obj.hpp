#ifndef INCLUDE_GRAPHICS_VERTEX_OBJ_HPP
#define INCLUDE_GRAPHICS_VERTEX_OBJ_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "utils.hpp"

namespace triangle {

struct Vertex_obj {
    GLuint vao_blue_;
    GLuint vao_red_;
    GLuint vbo_blue_;
    GLuint vbo_red_;

    Vertex_obj(std::vector<float> &blue_vertices,
              std::vector<float> &red_vertices) {
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    check_GL_error("glVertexAttribPointer");

    glBindVertexArray(vao_red_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_red_);
    glBufferData(GL_ARRAY_BUFFER, red_vertices.size() * sizeof(float), red_vertices.data(),
                 GL_STATIC_DRAW);
    check_GL_error("glBufferData");

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    check_GL_error("glVertexAttribPointer");

    }


};



} // namespace triangle

#endif // INCLUDE_GRAPHICS_VERTEX_OBJ_HPP