//
// Created by ianpo on 18/07/2023.
//

#pragma once

#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 1
#endif

#include "Voxymore/Layers/Layer.hpp"
#include "Voxymore/Events/MouseEvent.hpp"
#include "Voxymore/Events/KeyEvent.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"

#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 1
#endif

#include <imgui.h>

namespace Voxymore::Core {

    class ImGUILayer : public Layer {
    public:
        ImGUILayer();
        ~ImGUILayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;
        void OnImGuiRender() override;

        void Begin();
        void End();

        inline void SetBlockEvents(bool blockEvents) {m_BlockEvents = blockEvents;}
        inline bool GetBlockEvents() const {return m_BlockEvents;}
    private:
        bool m_BlockEvents = true;
        float m_Time = 0.0f;
    };

} // Core
