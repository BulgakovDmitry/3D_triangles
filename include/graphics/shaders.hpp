#ifndef INCLUDE_GRAPHICS_SHADERS_HPP
#define INCLUDE_GRAPHICS_SHADERS_HPP

namespace triangle {

inline const char *vertex_shader_source = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;

    uniform mat4 u_model;
    uniform mat4 u_view;
    uniform mat4 u_projection;

    void main() {
        gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
    }
)";

inline const char *fragment_shader_source = R"(
    #version 460 core
    out vec4 frag_color;

    void main() {
        frag_color = vec4(0.2, 0.6, 1.0, 1.0); // просто синий цвет
    }
)";

} // namespace triangle

#endif // INCLUDE_GRAPHICS_SHADERS_HPP
