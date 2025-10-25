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
    glm::vec3 position; 
    glm::vec3 world_up; 

    /*———————————————————————————— direction vectors and angles ————————————————————*/
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    float yaw, pitch;
    /*——————————————————————————————————————————————————————————————————————————————*/

    /*—————————————————————————————— motion parameters —————————————————————————————*/
    float movement_zoom_speed;
    float movement_speed;
    float mouse_sensitivity;
    float zoom;
    /*——————————————————————————————————————————————————————————————————————————————*/

    void update_camera();

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f)
        : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_zoom_speed(15.0f), movement_speed(2.5f),
          mouse_sensitivity(0.1f), zoom(30.0f) {
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
        zoom_in,
        zoom_out,
    };

    glm::mat4 get_view_matrix() { return glm::lookAt(position, position + front, up); }

    void process_keyboard(Camera_movement direction, float delta_time);
    void process_mouse_movement(float xoffset, float yoffset, bool constrainPitch = true); // TODO
    void process_mouse_scroll(Camera_movement direction, float delta_time);

    float get_zoom() const { return zoom; }
    float get_yaw() const { return yaw; }
    float get_pitch() const { return pitch; }
    glm::vec3 get_position() const { return position; }
    glm::vec3 get_front() const { return front; }
    glm::vec3 get_up() const { return up; }
    glm::vec3 get_right() const { return right; }
};

void Camera::update_camera() {
    glm::vec3 new_front;

    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(new_front);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::process_keyboard(Camera_movement direction, float delta_time) {
    float velocity = movement_speed * delta_time;

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

void Camera::process_mouse_scroll(Camera_movement direction, float delta_time) {
    float velocity = movement_zoom_speed * delta_time;

    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 90.0f)
        zoom = 90.0f;

    switch (direction) {
    case Camera_movement::zoom_in: {
        zoom -= velocity;
        break;
    }
    case Camera_movement::zoom_out: {
        zoom += velocity;
        break;
    }
    }
}

inline void Camera::process_mouse_movement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    update_camera();
}

} // namespace triangle

#endif // INCLUDE_GRAPHICS_CAMERA_HPP
