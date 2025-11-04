#ifndef GRAPHICS_DRIVER_HPP
#define GRAPHICS_DRIVER_HPP

#include <glm/ext/matrix_clip_space.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstddef>
#include <glad/glad.h> // TODO сделать его в проекте
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "graphics/camera.hpp"
#include "graphics/mesh.hpp"
#include "graphics/shader_program.hpp"
#include "graphics/shaders.hpp"
#include "graphics/utils.hpp"
#include "graphics/window.hpp"
#include "primitives/triangle.hpp"

namespace triangle {

class Graphics_driver {
  private:
    Window window_;

    Mesh blue_mesh_;
    Mesh red_mesh_;

    Shader_program shader_;

    Camera camera_;

    std::vector<float> vec1_;
    std::vector<float> vec2_;

  public:
    Graphics_driver(std::vector<float> &&vec1, std::vector<float> &&vec2)
        : vec1_{std::move(vec1)}, vec2_{std::move(vec2)} {

        glfwSetWindowUserPointer(window_, this); 
        glfwSetScrollCallback(window_, &Graphics_driver::static_scroll_callback);
        glfwSetCursorPosCallback(window_, &Graphics_driver::static_cursor_position_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        glEnable(GL_DEPTH_TEST);

        blue_mesh_.init_from_positions(vec1_, 0, 3);
        red_mesh_.init_from_positions(vec2_, 0, 3);

        check_GL_error("glGenBuffers");
        check_GL_error("glGenBuffers");
        check_GL_error("glBufferData");
        check_GL_error("glVertexAttribPointer");
        check_GL_error("glBufferData");
        check_GL_error("glVertexAttribPointer");

        if (!shader_.init()) {
            throw std::runtime_error("Enable to create and compile shader program");
        }
    }
    ~Graphics_driver() = default;

    Graphics_driver(const Graphics_driver &) = delete;
    Graphics_driver &operator=(const Graphics_driver &) = delete;
    Graphics_driver(Graphics_driver &&other) = default;
    Graphics_driver &operator=(Graphics_driver &&other) = default;

    void graphics_run();

  private:
    bool first_mouse_ = true;
    double last_x_ = 0.0;
    double last_y_ = 0.0;

    /*————————————————————— callbacks ——————————————————————————————————————————————*/
    static void static_scroll_callback(GLFWwindow *w, double xoffset, double yoffset);
    static void static_cursor_position_callback(GLFWwindow *w, double xpos, double ypos);
    /*——————————————————————————————————————————————————————————————————————————————*/

    /*————————————————————— event handlers —————————————————————————————————————————*/
    void on_scroll(double yoffset);
    void on_cursor_position(double xpos, double ypos);
    /*——————————————————————————————————————————————————————————————————————————————*/

    void process_input(float delta_time);
};

/*—————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
/*                                                                                                                 */
/*                                             implementation of methods */
/*                                                                                                                 */
/*—————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

static float get_delta_time(float &last_frame, float &current_frame) {
    current_frame = glfwGetTime();
    auto delta_time = current_frame - last_frame;
    last_frame = current_frame;

    return delta_time;
}

inline void Graphics_driver::graphics_run() {
    GLint material_color_loc = glGetUniformLocation(shader_.get_shader_program(), "material_color");
    if (material_color_loc == -1) {
        std::cerr << "ERROR: Uniform 'material_color' not found in shader_program" << std::endl;
        return;
    }

    GLint view_loc = glGetUniformLocation(shader_.get_shader_program(), "view");
    if (view_loc == -1) {
        std::cerr << "ERROR: Uniform 'view' not found in shader_program" << std::endl;
        return;
    }

    GLint projection_loc = glGetUniformLocation(shader_.get_shader_program(), "projection");
    if (projection_loc == -1) {
        std::cerr << "ERROR: Uniform 'projection' not found in shader_program" << std::endl;
        return;
    }

    float last_frame;
    float current_frame;

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        auto delta_time = get_delta_time(last_frame, current_frame);

        process_input(delta_time);

        glm::mat4 view = camera_.get_view_matrix();

        glm::mat4 projection =
            glm::perspective(glm::radians(camera_.get_zoom()), 1000.0f / 800.0f, 0.1f, 100.0f);

        glClearColor(0.75f, 0.85f, 0.90f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_.get_shader_program());
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

        if (!vec1_.empty()) {
            blue_mesh_.bind();
            glUniform3f(material_color_loc, 0.30f, 0.50f, 0.60f);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLint>(vec1_.size() / 3));
        }

        if (!vec2_.empty()) {
            red_mesh_.bind();
            glUniform3f(material_color_loc, 0.70f, 0.35f, 0.25f);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLint>(vec2_.size() / 3));
        }

        glfwSwapBuffers(window_);
    }
}

inline void Graphics_driver::static_scroll_callback(GLFWwindow *w, double /*xoffset*/,
                                                    double yoffset) {
    if (auto *self = static_cast<Graphics_driver *>(glfwGetWindowUserPointer(w))) {
        self->on_scroll(yoffset);
    }
}

inline void Graphics_driver::static_cursor_position_callback(GLFWwindow *w, double xpos,
                                                             double ypos) {
    if (auto *self = static_cast<Graphics_driver *>(glfwGetWindowUserPointer(w))) {
        self->on_cursor_position(xpos, ypos);
    }
}

inline void Graphics_driver::on_scroll(double yoffset) {
    const float step_dt = static_cast<float>(std::abs(yoffset)) * 0.08f;

    if (yoffset > 0.0) {
        camera_.process_mouse_scroll(Camera::Camera_movement::zoom_in, step_dt);
    } else if (yoffset < 0.0) {
        camera_.process_mouse_scroll(Camera::Camera_movement::zoom_out, step_dt);
    }
}

inline void Graphics_driver::on_cursor_position(double xpos, double ypos) {
    if (glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
        first_mouse_ = true;
        return;
    }

    if (first_mouse_) {
        last_x_ = xpos;
        last_y_ = ypos;
        first_mouse_ = false;
        return;
    }

    float xoffset = static_cast<float>(xpos - last_x_);
    float yoffset = static_cast<float>(last_y_ - ypos);

    last_x_ = xpos;
    last_y_ = ypos;

    camera_.process_mouse_movement(xoffset, yoffset, true);
}

void Graphics_driver::process_input(float delta_time) {
    if (glfwGetKey(window_, GLFW_KEY_EQUAL) == GLFW_PRESS)
        camera_.process_mouse_scroll(Camera::Camera_movement::zoom_in, delta_time);
    if (glfwGetKey(window_, GLFW_KEY_MINUS) == GLFW_PRESS)
        camera_.process_mouse_scroll(Camera::Camera_movement::zoom_out, delta_time);

    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
        camera_.process_keyboard(Camera::Camera_movement::up, delta_time);
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
        camera_.process_keyboard(Camera::Camera_movement::down, delta_time);
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
        camera_.process_keyboard(Camera::Camera_movement::left, delta_time);
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
        camera_.process_keyboard(Camera::Camera_movement::right, delta_time);
    if (glfwGetKey(window_, GLFW_KEY_PAGE_UP) == GLFW_PRESS ||
        glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS)
        camera_.process_keyboard(Camera::Camera_movement::forward, delta_time);
    if (glfwGetKey(window_, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS ||
        glfwGetKey(window_, GLFW_KEY_X) == GLFW_PRESS)
        camera_.process_keyboard(Camera::Camera_movement::backward, delta_time);
}

} // namespace triangle

#endif // GRAPHICS_DRIVER_HPP
