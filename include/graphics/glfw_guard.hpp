#ifndef INCLUDE_GLFW_GUARD_HPP
#define INCLUDE_GLFW_GUARD_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdexcept>

namespace triangle {

struct GlfwGuard {
    GlfwGuard() {
        try {
            if (!glfwInit())
                throw std::runtime_error("Failed to initialize GLFW");
        } catch (...) {
            glfwTerminate();
            throw;
        }
    }
    ~GlfwGuard() { glfwTerminate(); }

    GlfwGuard(const GlfwGuard &) = delete;
    GlfwGuard &operator=(const GlfwGuard &) = delete;
    GlfwGuard(GlfwGuard &&) = delete;
    GlfwGuard &operator=(GlfwGuard &&) = delete;
};

} // namespace triangle

#endif // INCLUDE_GLFW_GUARD_HPP