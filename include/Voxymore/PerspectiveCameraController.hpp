//
// Created by ianpo on 30/07/2023.
//

#pragma once

#include "Voxymore/Core.hpp"
#include "Voxymore/KeyCodes.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Layers/Layer.hpp"
#include "Voxymore/Events/Event.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"
#include "Voxymore/Events/KeyEvent.hpp"
#include "Voxymore/Events/MouseEvent.hpp"
#include "Voxymore/Renderer/PerspectiveCamera.hpp"

namespace Voxymore::Core {
    class PerspectiveCameraController : public Layer {
    public:
        PerspectiveCameraController(float width, float height);

        virtual void OnUpdate(TimeStep ts) override;
        virtual void OnEvent(Event& e) override;

        inline PerspectiveCamera& GetCamera() { return m_Camera; }
        inline const PerspectiveCamera& GetCamera() const { return m_Camera; }
    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);
        bool OnMouseMoved(MouseMovedEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    public:
        inline bool GetEnable() const { return m_Enable; }
        void SetEnable(bool isEnable);
    private:
        bool m_Enable = true;
    private:
        const float c_ScrollStep = 0.5f;
        const float c_TranslationSpeed = 5.0f;
        const float c_RotationSpeed = 0.5f;
        float m_Width;
        float m_Height;
        float m_FOV = 45.0f;
        float m_NearClip = 0.01f;
        float m_FarClip = 1000.0f;

        glm::vec3 m_Position = glm::zero<glm::vec3>();
        glm::quat m_Rotation = glm::identity<glm::quat>();
    private:
        bool m_HasSetMouse = false;
        glm::vec3 m_Movement = glm::zero<glm::vec3>();
        float m_MouseX, m_MouseY;
        float m_Sensitivity = 1.0f;
    private:
        KeyCode m_ForwardKey = KeyCode::KEY_W;
        KeyCode m_BackwardKey = KeyCode::KEY_S;
        KeyCode m_RightKey = KeyCode::KEY_D;
        KeyCode m_LeftKey = KeyCode::KEY_A;
        KeyCode m_UpKey = KeyCode::KEY_E;
        KeyCode m_DownKey = KeyCode::KEY_Q;
    private:
        PerspectiveCamera m_Camera;
    };

} // Core
