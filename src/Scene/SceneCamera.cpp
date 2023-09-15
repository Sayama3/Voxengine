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
		}

		SceneCamera::SceneCamera(float radianFov, float nearClip, float farClip, uint32_t width, uint32_t height) :
			m_AspectRatio((float)width / (float)height),
			m_IsOrthographic(false), m_PerspectiveVerticalFOV(radianFov),
			m_PerspectiveNear(nearClip),
			m_PerspectiveFar(farClip)
		{
		}

		SceneCamera::SceneCamera()
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
		}

		void SceneCamera::SetPerspective(float radianVerticalFov, float nearClip, float farClip)
		{
			m_IsOrthographic = false;
			m_PerspectiveVerticalFOV = radianVerticalFov;
			m_PerspectiveNear = nearClip;
			m_PerspectiveFar = farClip;
		}

		void SceneCamera::SwitchToPerspective(bool isPerspective)
		{
			m_IsOrthographic = !isPerspective;
		}

		void SceneCamera::SwitchToOrthographic(bool isOrthographic)
		{
			m_IsOrthographic = isOrthographic;
		}

		void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
		{
			VXM_CORE_ASSERT(width > 0 && height > 0, "Width({0}) and Height({1}) must be superior to 0.", width, height);
			m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
		}

		glm::mat4 SceneCamera::CalculateOrthographic() const
		{
			float left = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float right = +m_OrthographicSize * m_AspectRatio * 0.5f;
			float top = +m_OrthographicSize * 0.5f;
			float bottom = -m_OrthographicSize * 0.5f;
			return glm::ortho(left, right, bottom, top, m_OrthographicNear, m_OrthographicFar);
		}

		glm::mat4 SceneCamera::CalculatePerspective() const
		{
			return glm::perspective(m_PerspectiveVerticalFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}

		glm::mat4 SceneCamera::CalculateProjectionMatrix() const
		{
			return m_IsOrthographic ? CalculateOrthographic() : CalculatePerspective();
		}

		void SceneCamera::SetAspectRatio(float aspectRatio)
		{
			VXM_CORE_ASSERT(aspectRatio != 0.0f, "Aspect Ratio cannot be 0.");
			m_AspectRatio = aspectRatio;
		}
		glm::mat4 SceneCamera::GetProjectionMatrix() const
		{
			return CalculateProjectionMatrix();
		}
	}// namespace Core
}// namespace Voxymore