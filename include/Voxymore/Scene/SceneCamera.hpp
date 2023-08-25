//
// Created by ianpo on 24/08/2023.
//

#pragma once

#include "Voxymore/Renderer/Camera.hpp"

namespace Voxymore::Core
{
	//TODO: Split between Perspective and Orthographic.
	class SceneCamera : public Camera
	{
	public:
		/**
		 * Orthographic Camera Constructor.
		 * @param width
		 * @param height
		 * @param size
		 * @param nearClip
		 * @param farClip
		 */
		SceneCamera(uint32_t width, uint32_t height, float size, float nearClip, float farClip);
		/**
		 * Perspective Camera Constructor.
		 * @param radianFov
		 * @param nearClip
		 * @param farClip
		 * @param width
		 * @param height
		 */
		SceneCamera(float radianFov, float nearClip, float farClip, uint32_t width, uint32_t height);
		SceneCamera();
		virtual ~SceneCamera() override;

		void SetViewportSize(uint32_t width, uint32_t height);
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float radianFov, float nearClip, float farClip);
		void SwitchToPerspective(bool value);
	public:
		inline bool IsOrthographic() const { return m_IsOrthographic; }
		inline float GetAspectRatio() const { return m_AspectRatio; }

		inline float GetOrthographicSize() const { return m_OrthographicSize; }
		inline float GetOrthographicNear() const { return m_OrthographicNear; }
		inline float GetOrthographicFar() const { return m_OrthographicFar; }

		inline float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		inline float GetPerspectiveFOVDegree() const { return glm::degrees(m_PerspectiveFOV); }
		inline float GetPerspectiveNear() const { return m_PerspectiveNear; }
		inline float GetPerspectiveFar() const { return m_PerspectiveFar; }
	private:
		void CalculateProjectionMatrix();
		void CalculateOrthographic();
		void CalculatePerspective();
	private:
		float m_AspectRatio = 16.0f/9.0f;
		bool m_IsOrthographic = false;
	private:
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;
	private:
		float m_PerspectiveFOV = glm::radians(60.0f);
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 1000.0f;
	};

} // namespace Voxymore::Core

