#ifndef INCLUDE_GRAPHICS_WINDOW_HPP
#define INCLUDE_GRAPHICS_WINDOW_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

namespace triangle {
struct Window {
    GLFWwindow *window_;

    Window() {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        window_ = glfwCreateWindow(1000, 800, "OpenGL 4.6 Window", nullptr, nullptr);
        if (window_ == nullptr) {
            throw std::runtime_error("Create window failed");
        }

        glfwMakeContextCurrent(window_);
        glfwSetWindowUserPointer(window_, this);
    }

    ~Window() {
        if (window_) 
            glfwDestroyWindow(window_);
    }

    operator GLFWwindow *() const noexcept { return window_; }

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    Window(Window &&other) noexcept : window_(std::exchange(other.window_, nullptr)) {}

    Window &operator=(Window &&other) noexcept {
        if (this != &other) {
            if (window_) {
                glfwDestroyWindow(window_);
            }

            window_ = other.window_;
            other.window_ = nullptr;

            if (window_) {
                glfwSetWindowUserPointer(window_, this);
            }
        }
        return *this;
    }
};

} // namespace triangle

#endif // INCLUDE_WINDOW_HPP