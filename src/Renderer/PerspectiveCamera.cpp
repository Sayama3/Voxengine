//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/PerspectiveCamera.hpp"
#include "Voxymore/Application.hpp"

namespace Voxymore {
    namespace Core {

		PerspectiveCamera::PerspectiveCamera() {

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
			// Update View Matrix
			auto trsMatrix = glm::translate(glm::identity<glm::mat4>(), m_Position);
			trsMatrix = trsMatrix * glm::toMat4(m_Rotation);
			trsMatrix = glm::scale(trsMatrix, m_Scale);
			m_ViewMatrix = trsMatrix;
			UpdateViewProjectionMatrix();
		}

		void PerspectiveCamera::UpdateProjectionMatrix() {
			// Update View Matrix
			m_ProjectionMatrix = glm::perspectiveFov(m_FOV, m_Width, m_Height, m_NearClip, m_FarClip);
			UpdateViewProjectionMatrix();
		}

		void PerspectiveCamera::UpdateViewProjectionMatrix() {
			m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		}
	} // Voxymore
} // Core