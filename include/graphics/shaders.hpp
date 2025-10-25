#ifndef INCLUDE_GRAPHICS_SHADERS_HPP
#define INCLUDE_GRAPHICS_SHADERS_HPP

namespace triangle {

inline const char *vertex_shader_source = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;

    out vec3 normal;
    uniform mat4 u_model;
    uniform mat4 u_view;
    uniform mat4 u_projection;

    void main() {
        gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
        normal = normalize(aPos);
    }
)";

inline const char *fragment_shader_source = R"(
    #version 460 core
    out vec4 frag_color;
    in vec3 normal;

    uniform vec3 material_color;

    void main() {
        vec3 light_direction = normalize(vec3(0.5, 1.0, 0.8));

        vec3 ambient = vec3(0.2);

        float diffuse = max(dot(normalize(normal), light_direction), 0.0);

        vec3 result = (ambient + diffuse) * material_color;
        frag_color = vec4(result, 1.0);
    }
)";

} // namespace triangle

#endif // INCLUDE_GRAPHICS_SHADERS_HPP
