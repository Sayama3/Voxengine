//
// Created by ianpo on 30/07/2023.
//

#include "Voxymore/Core/PerspectiveCameraController.hpp"
#include "Voxymore/Core/Input.hpp"
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
        dispatcher.Dispatch<Voxymore::Core::WindowResizeEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnWindowResize));
        if(m_Enable)
        {
            dispatcher.Dispatch<Voxymore::Core::MouseScrolledEvent>(BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
        }
    }

    void PerspectiveCameraController::OnUpdate(TimeStep ts) {
        VXM_PROFILE_FUNCTION();

        if(m_Enable)
        {
            UpdateMovement();
            UpdateRotation();
        }

        m_Position += (m_Rotation * m_Movement) * (ts * c_TranslationSpeed);
        m_Camera.SetViewMatrix(m_Position, m_Rotation);
        if(m_UpdateProjectionMatrix){
            m_Camera.SetProjectionMatrix(m_Width, m_Height, m_FOV, m_NearClip, m_FarClip);
        }
    }

    void PerspectiveCameraController::SetEnable(bool isEnable)
    {
        if(isEnable != m_Enable)
        {
            m_Movement = {0, 0, 0};
            m_HasSetMouse = false;
            m_Enable = isEnable;
        }
    }

    bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent &e) {
        VXM_PROFILE_FUNCTION();
        if (m_Enable) {
            m_Position += glm::vec3(e.GetXOffset(),0.0f, e.GetYOffset()) * c_ScrollStep;
        }
        return false;
    }

    bool PerspectiveCameraController::OnWindowResize(WindowResizeEvent &e) {
        VXM_PROFILE_FUNCTION();
        if(e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            return false;
        }

        SetSize(e.GetWidth(), e.GetHeight());
        return false;
    }

    void PerspectiveCameraController::SetHeight(uint32_t height) {
        VXM_CORE_ASSERT(height > 0, "Camera height must be superior to 0.");
        m_Height = height;
        m_UpdateProjectionMatrix = true;
    }

    void PerspectiveCameraController::SetWidth(uint32_t width) {
        VXM_CORE_ASSERT(width > 0, "Camera width must be superior to 0.");
        m_Width = width;
        m_UpdateProjectionMatrix = true;
    }

    void PerspectiveCameraController::SetSize(uint32_t width, uint32_t height) {
        VXM_CORE_ASSERT(width > 0, "Camera width must be superior to 0.");
        VXM_CORE_ASSERT(height > 0, "Camera height must be superior to 0.");
        m_Width = width;
        m_Height = height;
        m_UpdateProjectionMatrix = true;
    }

    void PerspectiveCameraController::UpdateMovement() {
        VXM_PROFILE_FUNCTION();
        m_Movement = glm::vec3(0);
        if (Input::IsKeyPressed(m_ForwardKey)) {
            VXM_CORE_INFO("Press KEY W.");
            m_Movement += glm::vec3(0, 0, -1);
        }
        if (Input::IsKeyPressed(m_BackwardKey)) {
            VXM_CORE_INFO("Press KEY S.");
            m_Movement += glm::vec3(0, 0, 1);
        }
        if (Input::IsKeyPressed(m_RightKey)) {
            VXM_CORE_INFO("Press KEY D.");
            m_Movement += glm::vec3(1, 0, 0);
        }
        if (Input::IsKeyPressed(m_LeftKey)) {
            VXM_CORE_INFO("Press KEY A.");
            m_Movement += glm::vec3(-1, 0, 0);
        }
        if (Input::IsKeyPressed(m_UpKey)) {
            VXM_CORE_INFO("Press KEY E.");
            m_Movement += glm::vec3(0, 1, 0);
        }
        if (Input::IsKeyPressed(m_DownKey)) {
            VXM_CORE_INFO("Press KEY Q.");
            m_Movement += glm::vec3(0, -1, 0);
        }
    }

    void PerspectiveCameraController::UpdateRotation() {
        VXM_PROFILE_FUNCTION();
        auto[x, y] = Input::GetMousePosition();
        if (!m_HasSetMouse) {
            m_MouseX = x;
            m_MouseY = y;
            m_HasSetMouse = true;
            return;
        }
        glm::vec2 currentPos = {x, y};
        glm::vec2 olPos = {m_MouseX, m_MouseY};

        glm::vec2 deltaMove = currentPos - olPos;

        // old pos.
        m_MouseX = x;
        m_MouseY = y;

        VXM_INFO("Mouse Delta: x:{0}, y: {1}", deltaMove.x, deltaMove.y);
        glm::vec2 movement = glm::radians(glm::vec2(deltaMove.x, deltaMove.y)) * (m_Sensitivity * c_RotationSpeed);
        glm::vec3 right = glm::vec3{1, 0, 0};
        glm::vec3 up = {0, 1, 0};
        auto upwardRotation = glm::rotate(glm::identity<glm::quat>(), -movement.y, right); // Inverse because that's the way it works.
        auto rightwardRotation = glm::rotate(glm::identity<glm::quat>(), -movement.x, up); // Inverse because that's the way it works.
        m_Rotation = m_Rotation * upwardRotation; // Around the local right axis of the current rotation.
        m_Rotation = rightwardRotation * m_Rotation; // Around the global up axis.
    }

} // Core