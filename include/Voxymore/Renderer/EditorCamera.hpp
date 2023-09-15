//
// Created by ianpo on 15/09/2023.
//

#pragma once

#include "Voxymore/Renderer/Camera.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Events/Event.hpp"
#include "Voxymore/Events/MouseEvent.hpp"
#include "Voxymore/Core/KeyCodes.hpp"

namespace Voxymore::Core
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		inline virtual glm::mat4 GetProjectionMatrix() const override {return m_ProjectionMatrix;};
		glm::mat4 GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		inline float GetPitch() const { return m_Pitch; }
		inline float GetYaw() const { return m_Yaw; }

		inline bool GetViewportFocused() const { return m_ViewportFocused; }
		inline void SetViewportFocused(bool viewportFocused) { m_ViewportFocused = viewportFocused;}
		inline bool GetViewportHovered() const { return m_ViewportHovered; }
		inline void SetViewportHovered(bool viewportHovered) { m_ViewportHovered = viewportHovered;}
		inline void EnableMovement(bool enable) { m_EnableMovement = enable;}
		inline bool MovementIsEnable() const { return m_EnableMovement;}
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		void FirstPersonController(TimeStep ts, const glm::vec2& mouseDelta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		bool m_EnableMovement = true;
		bool m_MousePan = false, m_MouseRotate = false, m_FPS = false;
	public:
		Key m_BoostKey = Key::LeftShift;
		Key m_ForwardKey = Key::W;
		Key m_BackwardKey = Key::S;
		Key m_RightKey = Key::D;
		Key m_LeftKey = Key::A;
		Key m_UpKey = Key::E;
		Key m_DownKey = Key::Q;
	};

}// namespace Voxymore::Core
