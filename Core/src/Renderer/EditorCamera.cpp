//
// Created by ianpo on 15/09/2023.
//

#include "Voxymore/Renderer/EditorCamera.hpp"

#include "Voxymore/Core/Application.hpp"
#include "Voxymore/Core/Input.hpp"
#include "Voxymore/Core/MouseButtonCodes.hpp"
#include "Voxymore/Math/Math.hpp"

namespace Voxymore::Core
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), m_ProjectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		VXM_PROFILE_FUNCTION();
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		VXM_PROFILE_FUNCTION();
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		VXM_PROFILE_FUNCTION();
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		VXM_PROFILE_FUNCTION();
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		VXM_PROFILE_FUNCTION();
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		VXM_PROFILE_FUNCTION();
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::OnUpdate(TimeStep ts)
	{
		VXM_PROFILE_FUNCTION();
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;
		bool altPressed = Input::IsKeyPressed(Key::LeftAlt) ||Input::IsKeyPressed(Key::RightAlt);

		if (m_EnableMovement && !m_FPS && !m_MouseRotate && Input::IsMouseButtonPressed(Mouse::Middle))
		{
			m_MousePan = m_ViewportHovered || m_MousePan;
		}
		else
		{
			m_MousePan = false;
		}

		if (m_EnableMovement && altPressed && !m_MousePan && !m_FPS && Input::IsMouseButtonPressed(Mouse::Left))
		{
			m_MouseRotate = m_ViewportHovered || m_MouseRotate;
		}
		else
		{
			m_MouseRotate = false;
		}

		if (m_EnableMovement && !m_MousePan && !m_MouseRotate && Input::IsMouseButtonPressed(Mouse::Right))
		{
			m_FPS = m_ViewportHovered || m_FPS;
			Application::Get().GetWindow().SetCursorState(CursorState::Locked);
		}
		else
		{
			m_FPS = false;
			Application::Get().GetWindow().SetCursorState(CursorState::None);
		}

		if(m_MousePan)
		{
			MousePan(delta);
		} else if (m_MouseRotate)
		{
			MouseRotate(delta);
		} else if (m_FPS)
		{
			FirstPersonController(ts, delta);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		VXM_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(VXM_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		VXM_PROFILE_FUNCTION();
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		VXM_PROFILE_FUNCTION();
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		VXM_PROFILE_FUNCTION();
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		VXM_PROFILE_FUNCTION();
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	void EditorCamera::FirstPersonController(TimeStep ts, const glm::vec2& mouseDelta)
	{
		VXM_PROFILE_FUNCTION();
		// Rotation
		glm::vec3 oldPosition = CalculatePosition();
		MouseRotate(mouseDelta);
		glm::vec3 newPosition = CalculatePosition();
		glm::vec3 movement = newPosition - oldPosition;
		m_FocalPoint -= movement;

		// Translation
		glm::vec3 localMovement(0);
		bool boost = Input::IsKeyPressed(m_BoostKey);
		if (Input::IsKeyPressed(m_ForwardKey)) localMovement += glm::vec3(0, 0, 1);
		if (Input::IsKeyPressed(m_BackwardKey)) localMovement += glm::vec3(0, 0, -1);
		if (Input::IsKeyPressed(m_RightKey)) localMovement += glm::vec3(1, 0, 0);
		if (Input::IsKeyPressed(m_LeftKey)) localMovement += glm::vec3(-1, 0, 0);
		if (Input::IsKeyPressed(m_UpKey)) localMovement += glm::vec3(0, 1, 0);
		if (Input::IsKeyPressed(m_DownKey)) localMovement += glm::vec3(0, -1, 0);

		auto [xSpeed, ySpeed] = PanSpeed();
		float zSpeed = xSpeed;

		xSpeed *= 15.0f;
		ySpeed *= 15.0f;
		zSpeed *= 15.0f;

		m_FocalPoint += GetForwardDirection() * (localMovement.z * (boost ? zSpeed * 4.5f : zSpeed) * ts);
		m_FocalPoint += GetRightDirection() * (localMovement.x * (boost ? xSpeed * 4.5f : xSpeed) * ts);
		m_FocalPoint += GetUpDirection() * (localMovement.y * (boost ? ySpeed * 4.5f : ySpeed) * ts);
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		VXM_PROFILE_FUNCTION();
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		VXM_PROFILE_FUNCTION();
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		VXM_PROFILE_FUNCTION();
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
        VXM_PROFILE_FUNCTION();
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	glm::quat EditorCamera::GetOrientation() const
	{
        VXM_PROFILE_FUNCTION();
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	void EditorCamera::SetFocusTarget(const glm::vec3 &position)
	{
        VXM_PROFILE_FUNCTION();
		m_FocalPoint = position;
	}
}// namespace Voxymore::Core