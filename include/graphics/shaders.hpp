#ifndef INCLUDE_GRAPHICS_SHADERS_HPP
#define INCLUDE_GRAPHICS_SHADERS_HPP

namespace triangle {

inline const char *vertex_shader_source = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;

    uniform float u_time;

    out vec3 normal;

    void main() {
        float angle = u_time;
        float c = cos(angle);
        float s = sin(angle);

        mat3 rotation = mat3 (
            c, 0.0, -s,
            0.0, 1.0, 0.0,
            s, 0.0, c
        );

        vec3 rotated = rotation * aPos;
        gl_Position = vec4(rotated, 1.0);

        normal = normalize(rotated);

        // gl_Position = vec4(aPos, 1.0);
        // normal = normalize(aPos);
    }
)";

inline const char *fragment_shader_source = R"(
    #version 460 core
    in vec3 normal;
    out vec4 frag_color;

    void main() {
        vec3 light_direction = normalize(vec3(0.5, 1.0, 0.8));

        vec3 material_color = vec3(0.2, 0.6, 1.0);

        vec3 ambient = vec3(0.2);

        float diffuse = max(dot(normalize(normal), light_direction), 0.0);

        vec3 result = (ambient + diffuse) * material_color;
        frag_color = vec4(result, 1.0);
    }
)";

} // namespace triangle

#endif // INCLUDE_GRAPHICS_SHADERS_HPP
