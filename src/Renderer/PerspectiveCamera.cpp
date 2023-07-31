//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/PerspectiveCamera.hpp"
#include "Voxymore/Core/Application.hpp"

namespace Voxymore {
    namespace Core {

		PerspectiveCamera::PerspectiveCamera(float width, float height, float fov, float nearClip, float farClip, glm::vec3 position, glm::quat rotation)
		{
			// Update View Matrix (inversing the TRS matrix because
			// it's a camera, and therefore it move the world so every m_Movement is reverse.)
			m_ViewMatrix = glm::inverse(Math::TRS(position, rotation, glm::one<glm::vec3>()));

			// Update View Matrix
			m_ProjectionMatrix = glm::perspectiveFov(glm::radians<float>(fov), width, height, nearClip, farClip);

			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}


        glm::mat4 PerspectiveCamera::GetViewMatrix() const {
			return m_ViewMatrix;
        }

        glm::mat4 PerspectiveCamera::GetProjectionMatrix() const {
			return m_ProjectionMatrix;
        }

        glm::mat4 PerspectiveCamera::GetViewProjectionMatrix() const {
            return m_ViewProjectionMatrix;
        }

        void PerspectiveCamera::SetProjectionMatrix(float width, float height, float fov, float nearClip, float farClip) {
            m_ProjectionMatrix = glm::perspectiveFov(glm::radians<float>(fov), width, height, nearClip, farClip);
            UpdateViewProjectionMatrix();
        }

        void PerspectiveCamera::SetViewMatrix(glm::vec3 position, glm::quat rotation) {
            m_ViewMatrix = glm::inverse(Math::TRS(position, rotation, glm::one<glm::vec3>()));
            UpdateViewProjectionMatrix();
        }

		void PerspectiveCamera::UpdateViewProjectionMatrix() {
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}

		PerspectiveCamera::~PerspectiveCamera() {

		}
    } // Voxymore
} // Core