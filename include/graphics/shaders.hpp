#ifndef INCLUDE_GRAPHICS_SHADERS_HPP
#define INCLUDE_GRAPHICS_SHADERS_HPP

namespace triangle {

inline const char *vertex_shader_source = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 normal;

    out vec3 normal;

    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        gl_Position = projection * view * vec4(aPos, 1.0);
    }
)";

inline const char *fragment_shader_source = R"(
    #version 460 core
    in vec3 normal;
    out vec4 frag_color;

    uniform vec3 material_color;

    void main() {
        vec3 light_direction = normalize(vec3(0.5, 1.0, 0.8));

        vec3 ambient = vec3(0.01);

        float diffuse = max(dot(normalize(normal), light_direction), 0.0);

        vec3 result = (ambient + diffuse) * material_color;
        frag_color = vec4(result, 1.0);
    }
)";

} // namespace triangle

#endif // INCLUDE_GRAPHICS_SHADERS_HPP
