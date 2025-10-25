#ifndef INCLUDE_GRAPHICS_CAMERA_HPP
#define INCLUDE_GRAPHICS_CAMERA_HPP

#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

namespace triangle {

class OrbitCamera final {
  public:
    OrbitCamera() = default;

    glm::vec3 target{0.0f, 0.0f, 0.0f};
    float distance{3.0f};
    float min_distance{0.25f};
    float max_distance{50.0f};

    float yaw{0.0f};
    float pitch{0.0f};
    float fov_degrees{60.0f};
    float aspect{16.0f / 9.0f};
    float near_plane{0.1f};
    float far_plane{200.0f};

    // Чувствительность
    float rotate_sensitivity{0.0025f};
    float zoom_sensitivity{0.12f};

    // Подключение к окну GLFW. Вызывать после создания окна/контекста.
    void attach(GLFWwindow *window) {
        if (window_ == window)
            return;
        detach();

        window_ = window;
        if (!window_)
            return;

        // Цепочка: сохраняем прежние колбэки, ставим свои.
        prev_cursor_cb_ = glfwSetCursorPosCallback(window_, &OrbitCamera::cursor_trampoline);
        prev_scroll_cb_ = glfwSetScrollCallback(window_, &OrbitCamera::scroll_trampoline);
        prev_button_cb_ = glfwSetMouseButtonCallback(window_, &OrbitCamera::button_trampoline);
        prev_framebuffer_cb_ =
            glfwSetFramebufferSizeCallback(window_, &OrbitCamera::framebuffer_trampoline);

        instances()[window_] = this;

        // Актуализируем aspect
        int w{}, h{};
        glfwGetFramebufferSize(window_, &w, &h);
        if (h != 0)
            aspect = static_cast<float>(w) / static_cast<float>(h);

        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    void detach() {
        if (!window_)
            return;

        glfwSetCursorPosCallback(window_, prev_cursor_cb_);
        glfwSetScrollCallback(window_, prev_scroll_cb_);
        glfwSetMouseButtonCallback(window_, prev_button_cb_);
        glfwSetFramebufferSizeCallback(window_, prev_framebuffer_cb_);

        instances().erase(window_);
        window_ = nullptr;

        prev_cursor_cb_ = nullptr;
        prev_scroll_cb_ = nullptr;
        prev_button_cb_ = nullptr;
        prev_framebuffer_cb_ = nullptr;
    }

    [[nodiscard]] glm::mat4 view() const noexcept {
        return glm::lookAt(position(), target, glm::vec3{0.0f, 1.0f, 0.0f});
    }

    [[nodiscard]] glm::mat4 projection() const noexcept {
        return glm::perspective(glm::radians(fov_degrees), aspect, near_plane, far_plane);
    }

    [[nodiscard]] glm::vec3 position() const noexcept {
        const float cp = std::cos(pitch);
        const float sp = std::sin(pitch);
        const float cy = std::cos(yaw);
        const float sy = std::sin(yaw);
        const glm::vec3 offset{distance * cp * cy, distance * sp, distance * cp * sy};
        return target + offset;
    }

  private:
    // ----- инфраструктура для «чанинга» колбэков -----
    static std::unordered_map<GLFWwindow *, OrbitCamera *> &instances() {
        static std::unordered_map<GLFWwindow *, OrbitCamera *> map;
        return map;
    }
    static OrbitCamera *from(GLFWwindow *w) {
        auto it = instances().find(w);
        return it == instances().end() ? nullptr : it->second;
    }

    static void cursor_trampoline(GLFWwindow *w, double x, double y) {
        if (auto *self = from(w))
            self->on_cursor(x, y);
        if (auto *self = from(w); self && self->prev_cursor_cb_)
            self->prev_cursor_cb_(w, x, y);
    }
    static void scroll_trampoline(GLFWwindow *w, double xo, double yo) {
        if (auto *self = from(w))
            self->on_scroll(xo, yo);
        if (auto *self = from(w); self && self->prev_scroll_cb_)
            self->prev_scroll_cb_(w, xo, yo);
    }
    static void button_trampoline(GLFWwindow *w, int b, int a, int m) {
        if (auto *self = from(w))
            self->on_button(b, a, m);
        if (auto *self = from(w); self && self->prev_button_cb_)
            self->prev_button_cb_(w, b, a, m);
    }
    static void framebuffer_trampoline(GLFWwindow *w, int ww, int hh) {
        if (auto *self = from(w))
            self->on_framebuffer_resized(ww, hh);
        if (auto *self = from(w); self && self->prev_framebuffer_cb_)
            self->prev_framebuffer_cb_(w, ww, hh);
    }

    // ----- обработчики -----
    void on_cursor(double x, double y) {
        if (!rotating_) {
            last_ = {x, y};
            return;
        }
        const double dx = x - last_.x;
        const double dy = y - last_.y;

        yaw -= static_cast<float>(dx) * rotate_sensitivity;
        pitch -= static_cast<float>(dy) * rotate_sensitivity;

        // Не даём «вывернуться»
        constexpr float limit = 1.55334306f; // ~89°
        pitch = std::clamp(pitch, -limit, limit);

        last_ = {x, y};
    }

    void on_scroll(double, double yoff) {
        const float factor = std::exp(static_cast<float>(-yoff) * zoom_sensitivity);
        distance = std::clamp(distance * factor, min_distance, max_distance);
    }

    void on_button(int button, int action, int /*mods*/) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                rotating_ = true;
                glfwGetCursorPos(window_, &last_.x, &last_.y);
            } else if (action == GLFW_RELEASE) {
                rotating_ = false;
            }
        }
    }

    void on_framebuffer_resized(int width, int height) {
        if (height <= 0)
            return;
        aspect = static_cast<float>(width) / static_cast<float>(height);
        glViewport(0, 0, width, height);
    }

    struct Cursor {
        double x{0.0}, y{0.0};
    };

    GLFWwindow *window_{nullptr};
    Cursor last_{};
    bool rotating_{false};

    // Сохранённые «старые» колбэки
    GLFWcursorposfun prev_cursor_cb_{nullptr};
    GLFWscrollfun prev_scroll_cb_{nullptr};
    GLFWmousebuttonfun prev_button_cb_{nullptr};
    GLFWframebuffersizefun prev_framebuffer_cb_{nullptr};
};

} // namespace triangle

#endif // INCLUDE_GRAPHICS_CAMERA_HPP
