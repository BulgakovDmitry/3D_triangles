#ifndef GRAPHICS_DRIVER_HPP
#define GRAPHICS_DRIVER_HPP

#include <glm/ext/matrix_clip_space.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "graphics/camera.hpp"
#include "graphics/mesh.hpp"
#include "graphics/shader_program.hpp"
#include "graphics/window.hpp"

namespace triangle {

class Graphics_driver {
  private:
    struct Impl{
        std::vector<float> vec1_;
        std::vector<float> vec2_;

        Window window_;

        Mesh blue_mesh_;
        Mesh red_mesh_;

        Shader_program shader_;

        Camera camera_;

        bool first_mouse_ = true;
        double last_x_ = 0.0;
        double last_y_ = 0.0;

        Impl(std::vector<float>&& vec1, std::vector<float>&& vec2)
            : window_{}
            , vec1_{std::move(vec1)}
            , vec2_{std::move(vec2)}
            , blue_mesh_(vec1_, 0, 1, 3) 
            , red_mesh_(vec2_, 0, 1, 3) {}

        /*————————————————————— event handlers —————————————————————————————————————————*/
        void on_scroll(double yoffset);
        void on_cursor_position(double xpos, double ypos);
        /*——————————————————————————————————————————————————————————————————————————————*/
    };

   std::unique_ptr<Impl> impl;

  public:
    Graphics_driver(std::vector<float> &&vec1, std::vector<float> &&vec2)
        : impl{std::make_unique<Impl>(std::move(vec1), std::move(vec2))} {

        glfwSetWindowUserPointer(impl->window_, impl.get());
        glfwSetScrollCallback(impl->window_, &Graphics_driver::static_scroll_callback);
        glfwSetCursorPosCallback(impl->window_, &Graphics_driver::static_cursor_position_callback);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }

    ~Graphics_driver() = default;

    Graphics_driver(const Graphics_driver &) = delete;
    Graphics_driver &operator=(const Graphics_driver &) = delete;
    Graphics_driver(Graphics_driver &&other) = default;
    Graphics_driver &operator=(Graphics_driver &&other) = default;

    void graphics_run();

  private:

    /*————————————————————— callbacks ——————————————————————————————————————————————*/
    static void static_scroll_callback(GLFWwindow *w, double xoffset, double yoffset);
    static void static_cursor_position_callback(GLFWwindow *w, double xpos, double ypos);
    /*——————————————————————————————————————————————————————————————————————————————*/

    void process_input(float delta_time);
};

/*—————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
/*           implementation of methods */
/*—————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

static float get_delta_time(float &last_frame, float &current_frame) {
    current_frame = glfwGetTime();
    auto delta_time = current_frame - last_frame;
    last_frame = current_frame;

    return delta_time;
}

inline void Graphics_driver::graphics_run() {
    GLint material_color_loc = glGetUniformLocation(impl->shader_.get_shader_program(), "material_color");
    if (material_color_loc == -1) {
        std::cerr << "ERROR: Uniform 'material_color' not found in shader_program" << std::endl;
        return;
    }

    GLint view_loc = glGetUniformLocation(impl->shader_.get_shader_program(), "view");
    if (view_loc == -1) {
        std::cerr << "ERROR: Uniform 'view' not found in shader_program" << std::endl;
        return;
    }

    GLint projection_loc = glGetUniformLocation(impl->shader_.get_shader_program(), "projection");
    if (projection_loc == -1) {
        std::cerr << "ERROR: Uniform 'projection' not found in shader_program" << std::endl;
        return;
    }

    GLint model_loc = glGetUniformLocation(impl->shader_.get_shader_program(), "model");
    if (model_loc == -1) {
        std::cerr << "ERROR: Uniform 'model' not found in shader_program" << std::endl;
        return;
    }

    GLint light_direction_loc =
        glGetUniformLocation(impl->shader_.get_shader_program(), "light_direction");
    if (light_direction_loc == -1) {
        std::cerr << "ERROR: Uniform 'light_direction' not found in shader_program" << std::endl;
        return;
    }

    GLint view_pos_loc = glGetUniformLocation(impl->shader_.get_shader_program(), "view_pos");
    if (view_pos_loc == -1) {
        std::cerr << "ERROR: Uniform 'view_pos' not found in shader_program" << std::endl;
        return;
    }

    float last_frame;
    float current_frame;

    while (!glfwWindowShouldClose(impl->window_)) {
        glfwPollEvents();

        auto delta_time = get_delta_time(last_frame, current_frame);

        process_input(delta_time);

        glm::mat4 view = impl->camera_.get_view_matrix();

        glm::mat4 projection =
            glm::perspective(glm::radians(impl->camera_.get_zoom()), 1000.0f / 800.0f, 0.1f, 100.0f);

        glClearColor(0.75f, 0.85f, 0.90f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(impl->shader_.get_shader_program());
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::vec3 sun_direction = glm::normalize(glm::vec3(0.5f, -0.5f, 0.7f));
        glUniform3fv(light_direction_loc, 1, glm::value_ptr(sun_direction));

        glm::vec3 camera_pos = impl->camera_.get_position();
        glUniform3f(view_pos_loc, camera_pos.x, camera_pos.y, camera_pos.z);

        if (!impl->vec1_.empty()) {
            impl->blue_mesh_.bind();
            glUniform3f(material_color_loc, 0.30f, 0.50f, 0.60f);

            glm::mat4 model = glm::mat4(1.0f);
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLint>(impl->vec1_.size() / 3));
        }

        if (!impl->vec2_.empty()) {
            impl->red_mesh_.bind();
            glUniform3f(material_color_loc, 0.70f, 0.35f, 0.25f);

            glm::mat4 model = glm::mat4(1.0f);
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLint>(impl->vec2_.size() / 3));
        }

        glfwSwapBuffers(impl->window_);
    }
}

inline void Graphics_driver::static_scroll_callback(GLFWwindow *w, double /*xoffset*/,
                                                    double yoffset) {
    if (auto *self = static_cast<Impl *>(glfwGetWindowUserPointer(w))) {
        self->on_scroll(yoffset);
    }
}

inline void Graphics_driver::static_cursor_position_callback(GLFWwindow *w, double xpos,
                                                             double ypos) {
    if (auto *self = static_cast<Impl *>(glfwGetWindowUserPointer(w))) {
        self->on_cursor_position(xpos, ypos);
    }
}

inline void Graphics_driver::Impl::on_scroll(double yoffset) {
    const float step_dt = static_cast<float>(std::abs(yoffset)) * 0.08f;

    if (yoffset > 0.0) {
        camera_.process_mouse_scroll(Camera::Camera_movement::zoom_in, step_dt);
    } else if (yoffset < 0.0) {
        camera_.process_mouse_scroll(Camera::Camera_movement::zoom_out, step_dt);
    }
}

inline void Graphics_driver::Impl::on_cursor_position(double xpos, double ypos) {
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
    if (glfwGetKey(impl->window_, GLFW_KEY_EQUAL) == GLFW_PRESS)
        impl->camera_.process_mouse_scroll(Camera::Camera_movement::zoom_in, delta_time);
    if (glfwGetKey(impl->window_, GLFW_KEY_MINUS) == GLFW_PRESS)
        impl->camera_.process_mouse_scroll(Camera::Camera_movement::zoom_out, delta_time);

    if (glfwGetKey(impl->window_, GLFW_KEY_W) == GLFW_PRESS)
        impl->camera_.process_keyboard(Camera::Camera_movement::up, delta_time);
    if (glfwGetKey(impl->window_, GLFW_KEY_S) == GLFW_PRESS)
        impl->camera_.process_keyboard(Camera::Camera_movement::down, delta_time);
    if (glfwGetKey(impl->window_, GLFW_KEY_A) == GLFW_PRESS)
        impl->camera_.process_keyboard(Camera::Camera_movement::left, delta_time);
    if (glfwGetKey(impl->window_, GLFW_KEY_D) == GLFW_PRESS)
        impl->camera_.process_keyboard(Camera::Camera_movement::right, delta_time);
    if (glfwGetKey(impl->window_, GLFW_KEY_PAGE_UP) == GLFW_PRESS ||
        glfwGetKey(impl->window_, GLFW_KEY_Z) == GLFW_PRESS)
        impl->camera_.process_keyboard(Camera::Camera_movement::forward, delta_time);
    if (glfwGetKey(impl->window_, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS ||
        glfwGetKey(impl->window_, GLFW_KEY_X) == GLFW_PRESS)
        impl->camera_.process_keyboard(Camera::Camera_movement::backward, delta_time);
}

} // namespace triangle

#endif // GRAPHICS_DRIVER_HPP
