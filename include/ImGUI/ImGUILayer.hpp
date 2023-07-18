//
// Created by ianpo on 18/07/2023.
//

#pragma once

#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 1
#endif

#include "Layers/Layer.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/ApplicationEvent.hpp"

namespace Voxymore::Core {

    class VXM_API ImGUILayer : public Layer {
    public:
        ImGUILayer();
        ~ImGUILayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(Event& event) override;
    private:
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);
        bool OnWindowResizeEvent(WindowResizeEvent& e);
    private:
        float m_Time = 0.0f;
    };

} // Core
