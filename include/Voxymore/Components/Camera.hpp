//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core.hpp"
#include "Voxymore/Math.hpp"

namespace Voxymore {
    namespace Core {

        class Camera {
        public:
            glm::mat4 GetViewProjectionMatrix() const;
            glm::mat4 GetProjectionMatrix() const;
            glm::mat4 GetViewMatrix() const;
        private:
            // Transform components
            glm::vec3 m_Position = glm::zero<glm::vec3>();
            glm::quat m_Rotation = glm::identity<glm::quat>();
            glm::vec3 m_Scale = glm::one<glm::vec3>();
            // Perspective Camera components
            // In degrees.
            float m_FOV = 65.0f;
            float m_NearClip = 0.01f;
            float m_FarClip = 1000.0f;
        };

    } // Voxymore
} // Core
