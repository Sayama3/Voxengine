//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Components/Camera.hpp"
#include "Voxymore/Application.hpp"

namespace Voxymore {
    namespace Core {
        glm::mat4 Camera::GetViewMatrix() const {
            auto id = glm::identity<glm::mat4>();
            auto translate = glm::translate(id, m_Position);
            auto rotation = glm::toMat4(m_Rotation);
            auto scale = glm::scale(id, m_Scale);
            return translate * rotation * scale;
        }

        glm::mat4 Camera::GetProjectionMatrix() const {
            float width = Application::Get().GetWindow().GetWidth();
            float height = Application::Get().GetWindow().GetHeight();
            return glm::perspectiveFov(glm::radians(m_FOV), width, height, m_NearClip, m_FarClip);
        }

        glm::mat4 Camera::GetViewProjectionMatrix() const {
            return GetProjectionMatrix() * GetViewMatrix();
        }
    } // Voxymore
} // Core