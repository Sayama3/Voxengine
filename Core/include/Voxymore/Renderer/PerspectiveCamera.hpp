//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Renderer/Camera.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Math.hpp"

namespace Voxymore::Core {

    class PerspectiveCamera : Camera {
    public:
        PerspectiveCamera(float width, float height, float fov = 60.0f, float nearClip = 0.01f, float farClip = 1000.0f, glm::vec3 position = glm::zero<glm::vec3>(), glm::quat rotation = glm::identity<glm::quat>());
        virtual ~PerspectiveCamera() override;

        glm::mat4 GetViewProjectionMatrix() const;
		virtual glm::mat4 GetProjectionMatrix() const;
		glm::mat4 GetViewMatrix() const;


        void SetProjectionMatrix(float width, float height, float fov = 60.0f, float nearClip = 0.01f, float farClip = 1000.0f);
        void SetViewMatrix(glm::vec3 position, glm::quat rotation);
    private:
        void UpdateViewProjectionMatrix();
    private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
    };

} // Core
