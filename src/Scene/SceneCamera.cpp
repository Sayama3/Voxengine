//
// Created by ianpo on 24/08/2023.
//

#include "Voxymore/Scene/SceneCamera.hpp"

namespace Voxymore
{
	namespace Core
	{
		SceneCamera::SceneCamera(uint32_t width, uint32_t height, float size, float nearClip, float farClip) :
			m_AspectRatio(static_cast<float>(width) / static_cast<float>(height)),
			m_IsOrthographic(true),
			m_OrthographicSize(size),
			m_OrthographicNear(nearClip),
			m_OrthographicFar(farClip)
		{
			CalculateOrthographic();
		}

		SceneCamera::SceneCamera(float radianFov, float nearClip, float farClip, uint32_t width, uint32_t height) :
			m_AspectRatio((float)width / (float)height),
			m_IsOrthographic(false),
			m_PerspectiveFOV(radianFov),
			m_PerspectiveNear(nearClip),
			m_PerspectiveFar(farClip)
		{
			CalculatePerspective();
		}

		SceneCamera::SceneCamera() : Camera(glm::mat4(1.0f))
		{
		}

		SceneCamera::~SceneCamera()
		{
		}

		void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
		{
			m_IsOrthographic = true;
			m_OrthographicSize = size;
			m_OrthographicNear = nearClip;
			m_OrthographicFar = farClip;
			CalculateOrthographic();
		}

		void SceneCamera::SetPerspective(float radianFov, float nearClip, float farClip)
		{
			m_IsOrthographic = false;
			m_PerspectiveFOV = radianFov;
			m_PerspectiveNear = nearClip;
			m_PerspectiveFar = farClip;
			CalculatePerspective();
		}

		void SceneCamera::SwitchToPerspective(bool value)
		{
			m_IsOrthographic = !value;
			CalculateProjectionMatrix();
		}

		void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
		{
			VXM_CORE_ASSERT(width > 0 && height > 0, "Width({0}) and Height({1}) must be superior to 0.", width, height);
			m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
			CalculateProjectionMatrix();
		}

		void SceneCamera::CalculateOrthographic()
		{
			float left = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float right = +m_OrthographicSize * m_AspectRatio * 0.5f;
			float top = +m_OrthographicSize * 0.5f;
			float bottom = -m_OrthographicSize * 0.5f;
			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, m_OrthographicNear, m_OrthographicFar);
		}

		void SceneCamera::CalculatePerspective()
		{
			m_ProjectionMatrix = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}

		void SceneCamera::CalculateProjectionMatrix()
		{
			if(m_IsOrthographic) CalculateOrthographic();
			else CalculatePerspective();
		}
	}// namespace Core
}// namespace Voxymore