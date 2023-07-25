//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/PerspectiveCamera.hpp"
#include "Voxymore/Application.hpp"

namespace Voxymore {
    namespace Core {

		PerspectiveCamera::PerspectiveCamera(float width, float height, float fov, float nearClip, float farClip, glm::vec3 position, glm::quat rotation)
		 : m_Width(width),
         m_Height(height),
         m_FOV(fov),
         m_NearClip(nearClip),
         m_FarClip(farClip),
         m_Position(position),
         m_Rotation(rotation)
		{
			// Update View Matrix (inversing the TRS matrix because
			// it's a camera, and therefore it move the world so every movement is reverse.)
			m_ViewMatrix = glm::inverse(Math::TRS(m_Position, m_Rotation, glm::one<glm::vec3>()));

			// Update View Matrix
			m_ProjectionMatrix = glm::perspectiveFov(glm::radians<float>(m_FOV), m_Width, m_Height, m_NearClip, m_FarClip);

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

		void PerspectiveCamera::UpdateViewMatrix() {
			// Update View Matrix (inversing the TRS matrix because
			// it's a camera, and therefore it move the world so every movement is reverse.)
			m_ViewMatrix = glm::inverse(Math::TRS(m_Position, m_Rotation, m_Scale));
		}

		void PerspectiveCamera::UpdateProjectionMatrix() {
			// Update View Matrix
			m_ProjectionMatrix = glm::perspectiveFov(glm::radians<float>(m_FOV), m_Width, m_Height, m_NearClip, m_FarClip);
		}

		void PerspectiveCamera::UpdateViewProjectionMatrix() {
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}

        void PerspectiveCamera::UpdateAllMatrix() {
            UpdateViewMatrix();
            UpdateProjectionMatrix();
            UpdateViewProjectionMatrix();
        }

		PerspectiveCamera::~PerspectiveCamera() {

		}
	} // Voxymore
} // Core