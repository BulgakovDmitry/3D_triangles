#ifndef INCLUDE_GRAPHICS_SHADERS_HPP
#define INCLUDE_GRAPHICS_SHADERS_HPP

namespace triangle {

inline const char *vertex_shader_source = R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;

    out vec3 frag_pos;
    out vec3 normal;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        frag_pos = vec3(model * vec4(aPos, 1.0));
        normal = mat3(transpose(inverse(model))) * aNormal;
        gl_Position = projection * view * vec4(frag_pos, 1.0);
    }
)";

inline const char *fragment_shader_source = R"(
    #version 460 core

    in vec3 frag_pos;
    in vec3 normal;
    out vec4 frag_color;

    uniform vec3 material_color;
    uniform vec3 light_direction;
    uniform vec3 view_pos;

    void main() {
        vec3 norm = normalize(normal);
        vec3 light_dir = normalize(-light_direction);
        vec3 view_dir = normalize(view_pos - frag_pos);

        bool is_front = gl_FrontFacing;
        vec3 effective_normal = is_front ? norm : -norm;

        vec3 ambient = 0.5 * material_color;

        float diff = max(dot(effective_normal, light_dir), 0.0);
        vec3 diffuse = diff * (is_front ? material_color : material_color * 0.6);

        vec3 specular = vec3(0.0);
        if (is_front) {
            vec3 reflect_dir = reflect(-light_dir, norm);
            float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 8.0);
            specular = 0.5 * spec * vec3(1.0);
        }

        vec3 result = clamp(ambient + diffuse + specular, 0.0, 1.0);
        frag_color = vec4(result, 1.0);
    }
)";

} // namespace triangle

#endif // INCLUDE_GRAPHICS_SHADERS_HPP
