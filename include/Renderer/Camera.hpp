//
// Created by ianpo on 01/06/2023.
//

#ifndef LEARNOPENGL_CAMERA_HPP
#define LEARNOPENGL_CAMERA_HPP

namespace Voxymore::Core::Renderer {
// Default camera values
    const float c_YAW = -90.0f;
    const float c_PITCH = 0.0f;
    const float c_SPEED = 2.5f;
    const float c_SENSITIVITY = 0.1f;
    const float c_ZOOM = 60.0f;
    const float c_NEAR = 0.1f;
    const float c_FAR = 100.0f;

    class Camera {
    private:
        // camera Attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        // euler Angles
        bool firstMouse = true;
        float Yaw;
        float Pitch;
        // camera options
        float Zoom;
        float Near;
        float Far;
        // screen options
        glm::vec2 ScreenSize;
        // Ease of life
        glm::vec2 lastMousePosition;

        void updateCameraVectors();

    public:
        float MovementSpeed;
        float MouseSensitivity;

        explicit Camera(glm::vec2 _screenSize, glm::vec3 _position = {0.0f, 0.0f, 0.0f},
                        glm::vec3 _up = {0.0f, 1.0f, 0.0f}, float _yaw = c_YAW, float _pitch = c_PITCH, float _near = c_NEAR, float _far = c_FAR);

        void ProcessKeyboard(GLFWwindow *window, double deltaTime);

        void ProcessMouseCursor(glm::dvec2 pos, bool constrainPitch = true);

        void ProcessMouseScroll(glm::dvec2 offset);

        void OnScreenSizeChange(glm::ivec2 size);

        inline glm::mat4 GetViewMatrix() const {
            return glm::lookAt(Position, Position + Front, Up);
        }

        inline glm::mat4 GetProjectionMatrix() const {
            return glm::perspective(glm::radians(Zoom), ScreenSize.x / ScreenSize.y, Near, Far);
        }

        inline float GetZoom() const {
            return Zoom;
        }

        inline glm::vec3 GetPosition() const {
            return Position;
        }

        inline glm::vec3 GetForward() const {
            return Front;
        }

        inline glm::vec3 GetUp() const {
            return Up;
        }

        inline glm::vec3 GetRight() const {
            return Right;
        }

        inline void ResetCursor() {
            firstMouse = true;
        }
    };

}
#endif //LEARNOPENGL_CAMERA_HPP
