//
// Created by ianpo on 01/06/2023.
//

#include "Renderer/Camera.hpp"
#include "Renderer/GLFWType.hpp"

namespace Voxymore::Core::Renderer {
    Camera::Camera(glm::vec2 _screenSize, glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch, float _near, float _far) :
            Position(_position),
            Front(glm::vec3(0.0f, 0.0f, -1.0f)),
            Up(_up),
            Right(glm::normalize(glm::cross(Front, Up))),
            WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
            Yaw(_yaw),
            Pitch(_pitch),
            Zoom(c_ZOOM),
            Near(_near),
            Far(_far),
            ScreenSize(_screenSize),
            MovementSpeed(c_SPEED),
            MouseSensitivity(c_SENSITIVITY) {

    }

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void Camera::ProcessKeyboard(GLFWwindow *window, double deltaTime) {
        float velocity = MovementSpeed * static_cast<float>(deltaTime);
        auto cameraMovement = glm::vec3(0);
        if (glfwGetKey(window, GLFWKey::KEY_W) == GLFWKeyState::PRESS) {
            cameraMovement += Front;
        }
        if (glfwGetKey(window, GLFWKey::KEY_S) == GLFWKeyState::PRESS) {
            cameraMovement -= Front;
        }
        if (glfwGetKey(window, GLFWKey::KEY_A) == GLFWKeyState::PRESS) {
            cameraMovement -= Right;
        }
        if (glfwGetKey(window, GLFWKey::KEY_D) == GLFWKeyState::PRESS) {
            cameraMovement += Right;
        }
        if (glfwGetKey(window, GLFWKey::KEY_Q) == GLFWKeyState::PRESS) {
            cameraMovement -= Up;
        }
        if (glfwGetKey(window, GLFWKey::KEY_E) == GLFWKeyState::PRESS) {
            cameraMovement += Up;
        }

        Position += cameraMovement * velocity;
    }

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void Camera::ProcessMouseCursor(glm::dvec2 pos, bool constrainPitch) {
        if (firstMouse) // initially set to true
        {
            lastMousePosition.x = static_cast<float>(pos.x);
            lastMousePosition.y = static_cast<float>(pos.y);
            firstMouse = false;
        }
        glm::vec2 offset{
                static_cast<float>(pos.x) - lastMousePosition.x,
                lastMousePosition.y - static_cast<float>(pos.y)
        };
        lastMousePosition.x = static_cast<float>(pos.x);
        lastMousePosition.y = static_cast<float>(pos.y);

        offset *= MouseSensitivity;

        Yaw += static_cast<float>(offset.x);
        Pitch += static_cast<float>(offset.y);

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            else if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void Camera::ProcessMouseScroll(glm::dvec2 offset) {
        Zoom -= static_cast<float>(offset.y);
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        else if (Zoom > c_ZOOM)
            Zoom = c_ZOOM;
    }

// calculates the front vector from the Camera's (updated) Euler Angles
    void Camera::updateCameraVectors() {
        // calculate the new Front vector
        glm::vec3 front = {0.0f, 0.0f, 0.0f};
        front.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
        front.y = glm::sin(glm::radians(Pitch));
        front.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front,
                                          WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }

    void Camera::OnScreenSizeChange(glm::ivec2 size) {
        ScreenSize = size;
        glViewport(0, 0, size.x, size.y);
    }
}