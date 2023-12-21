//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/PerspectiveCamera.hpp"
#include "Voxymore/Core/Application.hpp"


namespace Voxymore::Core {

	PerspectiveCamera::PerspectiveCamera(float width, float height, float fov, float nearClip, float farClip, glm::vec3 position, glm::quat rotation) :
			m_ProjectionMatrix( glm::perspectiveFov(glm::radians<float>(fov), width, height, nearClip, farClip)),
			m_ViewMatrix(glm::inverse(Math::TRS(position, rotation, glm::one<glm::vec3>()))),
			m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix)
	{
	}


	glm::mat4 PerspectiveCamera::GetViewMatrix() const {
		VXM_PROFILE_FUNCTION();
		return m_ViewMatrix;
	}

	glm::mat4 PerspectiveCamera::GetViewProjectionMatrix() const {
		VXM_PROFILE_FUNCTION();
		return m_ViewProjectionMatrix;
	}
	glm::mat4 PerspectiveCamera::GetProjectionMatrix() const
	{
		VXM_PROFILE_FUNCTION();
		return m_ProjectionMatrix;
	}

	void PerspectiveCamera::SetProjectionMatrix(float width, float height, float fov, float nearClip, float farClip) {
		VXM_PROFILE_FUNCTION();
		m_ProjectionMatrix = glm::perspectiveFov(glm::radians<float>(fov), width, height, nearClip, farClip);
		UpdateViewProjectionMatrix();
	}

	void PerspectiveCamera::SetViewMatrix(glm::vec3 position, glm::quat rotation) {
		VXM_PROFILE_FUNCTION();
		m_ViewMatrix = glm::inverse(Math::TRS(position, rotation, glm::one<glm::vec3>()));
		UpdateViewProjectionMatrix();
	}

	void PerspectiveCamera::UpdateViewProjectionMatrix() {
		VXM_PROFILE_FUNCTION();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	PerspectiveCamera::~PerspectiveCamera() {

	}
} // Voxymore
// Core