#ifndef INCLUDE_GLFW_GUARD_HPP
#define INCLUDE_GLFW_GUARD_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdexcept>

namespace triangle {

struct GlfwGuard {
    GlfwGuard() {
        if (!glfwInit()) 
            throw std::runtime_error("Failed to initialize GLFW");
    }
    ~GlfwGuard() { glfwTerminate(); }

    GlfwGuard(const GlfwGuard&) = delete;
    GlfwGuard& operator=(const GlfwGuard&) = delete;
    GlfwGuard(GlfwGuard&&) = delete;
    GlfwGuard& operator=(GlfwGuard&&) = delete;
};

} // namespace triangle

#endif // INCLUDE_GLFW_GUARD_HPP