//
// Created by ianpo on 30/07/2023.
//

#include "Voxymore/PerspectiveCameraController.hpp"
#include <cfloat>

// TODO (01) : Add camera movement and rotation (WASD + Mouse) only when right clicking on the viewport
// TODO (02) : Make the PerspectiveCameraController a Layer that we only add when editing (and therefore remove in the final game).
// TODO (03) : Remove the ability to make a full 360 and somewhat lock the upper movement when we go too high (i.e. ~89.9°)
// TODO (04) : Add gizmos to show the developer what are the up and down relative to where he is looking

namespace Voxymore::Core {
    PerspectiveCameraController::PerspectiveCameraController(float width, float height)
    :
            Layer("Perspective Camera Controller"),
            m_Width(width),
            m_Height(height),
            m_Camera(m_Width, m_Height, m_FOV, m_NearClip, m_FarClip, m_Position, m_Rotation)
    {

    }


    void PerspectiveCameraController::OnEvent(Event &e) {
        Voxymore::Core::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Voxymore::Core::WindowResizeEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnWindowResize, std::placeholders::_1));
        dispatcher.Dispatch<Voxymore::Core::KeyPressedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnKeyPressed, std::placeholders::_1));
        dispatcher.Dispatch<Voxymore::Core::KeyReleasedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnKeyReleased, std::placeholders::_1));
        dispatcher.Dispatch<Voxymore::Core::MouseMovedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseMoved, std::placeholders::_1));
        dispatcher.Dispatch<Voxymore::Core::MouseScrolledEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled, std::placeholders::_1));
//        dispatcher.Dispatch<Voxymore::Core::MouseButtonPressedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseButtonPressed, std::placeholders::_1));
//        dispatcher.Dispatch<Voxymore::Core::MouseButtonReleasedEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseButtonReleased, std::placeholders::_1));
    }

    bool PerspectiveCameraController::OnKeyPressed(KeyPressedEvent &e) {
        if (e.GetKeyCode() == m_ForwardKey && e.GetRepeatCount() == 0) {
            VXM_CORE_INFO("Press KEY W.");
            m_Movement += glm::vec3(0, 0, -1);
        }
        if (e.GetKeyCode() == m_BackwardKey && e.GetRepeatCount() == 0) {
            VXM_CORE_INFO("Press KEY S.");
            m_Movement += glm::vec3(0, 0, 1);
        }
        if (e.GetKeyCode() == m_RightKey && e.GetRepeatCount() == 0) {
            VXM_CORE_INFO("Press KEY D.");
            m_Movement += glm::vec3(1, 0, 0);
        }
        if (e.GetKeyCode() == m_LeftKey && e.GetRepeatCount() == 0) {
            VXM_CORE_INFO("Press KEY A.");
            m_Movement += glm::vec3(-1, 0, 0);
        }
        if (e.GetKeyCode() == m_UpKey && e.GetRepeatCount() == 0) {
            VXM_CORE_INFO("Press KEY E.");
            m_Movement += glm::vec3(0, 1, 0);
        }
        if (e.GetKeyCode() == m_DownKey && e.GetRepeatCount() == 0) {
            VXM_CORE_INFO("Press KEY Q.");
            m_Movement += glm::vec3(0, -1, 0);
        }

        return false;
    }

    bool PerspectiveCameraController::OnKeyReleased(KeyReleasedEvent &e) {
        if(glm::length2(m_Movement) < FLT_EPSILON) return false;

        if (e.GetKeyCode() == m_ForwardKey) {
            VXM_CORE_INFO("Release KEY W.");
            m_Movement -= glm::vec3(0, 0, -1);
        }
        if (e.GetKeyCode() == m_BackwardKey) {
            VXM_CORE_INFO("Release KEY S.");
            m_Movement -= glm::vec3(0, 0, 1);
        }
        if (e.GetKeyCode() == m_RightKey) {
            VXM_CORE_INFO("Release KEY D.");
            m_Movement -= glm::vec3(1, 0, 0);
        }
        if (e.GetKeyCode() == m_LeftKey) {
            VXM_CORE_INFO("Release KEY A.");
            m_Movement -= glm::vec3(-1, 0, 0);
        }
        if (e.GetKeyCode() == m_UpKey) {
            VXM_CORE_INFO("Release KEY E.");
            m_Movement -= glm::vec3(0, 1, 0);
        }
        if (e.GetKeyCode() == m_DownKey) {
            VXM_CORE_INFO("Release KEY Q.");
            m_Movement -= glm::vec3(0, -1, 0);
        }
        return false;
    }

    bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent &event) {
        if (!m_HasSetMouse) {
            m_MouseX = event.GetX();
            m_MouseY = event.GetY();
            m_HasSetMouse = true;
            return false;
        }
        glm::vec2 currentPos = {event.GetX(), event.GetY()};
        glm::vec2 olPos = {m_MouseX, m_MouseY};

        glm::vec2 deltaMove = currentPos - olPos;

        // old pos.
        m_MouseX = event.GetX();
        m_MouseY = event.GetY();

        VXM_INFO("Mouse Delta: x:{0}, y: {1}", deltaMove.x, deltaMove.y);
        glm::vec2 movement = glm::radians(glm::vec2(deltaMove.x, deltaMove.y)) * (m_Sensitivity * c_RotationSpeed);
        glm::vec3 right = glm::vec3{1, 0, 0};
        glm::vec3 up = {0, 1, 0};
        auto upwardRotation = glm::rotate(glm::identity<glm::quat>(), -movement.y, right); // Inverse because that's the way it works.
        auto rightwardRotation = glm::rotate(glm::identity<glm::quat>(), -movement.x, up); // Inverse because that's the way it works.
        m_Rotation = m_Rotation * upwardRotation; // Around the local right axis of the current rotation.
        m_Rotation = rightwardRotation * m_Rotation; // Around the global up axis.

        return false;
    }

    void PerspectiveCameraController::OnUpdate(TimeStep ts) {
        m_Position += m_Movement * ts.GetSeconds() * c_TranslationSpeed;
        m_Camera.SetViewMatrix(m_Position, m_Rotation);
    }

    void PerspectiveCameraController::SetEnable(bool isEnable)
    {
        m_Movement = {0,0,0};
        m_HasSetMouse = false;
        m_Enable = isEnable;
    }

    bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent &e) {
        if (m_Enable) {
            m_Position += glm::vec3(e.GetXOffset(),0.0f, e.GetYOffset()) * c_ScrollStep;
        }
        return false;
    }

    bool PerspectiveCameraController::OnWindowResize(WindowResizeEvent &e) {
        m_Width = static_cast<float>(e.GetWidth());
        m_Height = static_cast<float>(e.GetHeight());
        m_Camera.SetProjectionMatrix(m_Width, m_Height, m_FOV, m_NearClip, m_FarClip);
        return false;
    }

} // Core