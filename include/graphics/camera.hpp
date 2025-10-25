#ifndef INCLUDE_GRAPHICS_CAMERA_HPP
#define INCLUDE_GRAPHICS_CAMERA_HPP

#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace triangle {

struct Camera {
    glm::vec3 position;  // текущая позиция камеры
    glm::vec3 world_up;  // мировая ориентация

    /* ———————————————————————————— направляющие векторы и углы —————————————————————*/
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    float yaw, pitch;
    /* ——————————————————————————————————————————————————————————————————————————————*/


    /* —————————————————————————————— параметры движения ————————————————————————————*/
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
    /* ——————————————————————————————————————————————————————————————————————————————*/


    void update_camera();


    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, float pitch = 0.0f)
           : front(glm::vec3(0.0f, 0.0f, -1.0f)),
             movementSpeed(2.5f),
             mouseSensitivity(0.1f),
             zoom(45.0f)
    {
        this->position = position;
        this->world_up = up;
        this->yaw = yaw;
        this->pitch = pitch;
        update_camera();
    }

    enum class Camera_movement {
        forward,
        backward,
        left,
        right,
        up,
        down,
    };

    glm::mat4 get_view_matrix() {
        return glm::lookAt(position, position + front, up);
    }


    void process_keyboard(Camera_movement direction, float delta_time);
    void process_mouse_movement(float xoffset, float yoffset, bool constrainPitch = true); //TODO
    void process_mouse_scroll(float yoffset); //TODO

    float get_zoom() const { return zoom; }
    float get_yaw() const { return yaw; }
    float get_pitch() const { return pitch; }
    glm::vec3 get_position() const { return position; }
    glm::vec3 get_front() const { return front; }
    glm::vec3 get_up() const { return up; }
    glm::vec3 get_right() const { return right; }
};

void Camera::update_camera() {
    glm::vec3 newFront;

    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::process_keyboard(Camera_movement direction, float delta_time) {
    float velocity = movementSpeed * delta_time;

    switch (direction) {
        case Camera_movement::forward: {
            position += front * velocity;
            break;
        }
        case Camera_movement::backward: {
            position -= front * velocity;
            break;
        }
        case Camera_movement::left: {
            position -= right * velocity;
            break;
        }
        case Camera_movement::right: {
            position += right * velocity;
            break;
        }
        case Camera_movement::up: {
            position += up * velocity;
            break;
        }
        case Camera_movement::down: {
            position -= up * velocity;
            break;
        }
    }
}

} // namespace triangle

#endif // INCLUDE_GRAPHICS_CAMERA_HPP
