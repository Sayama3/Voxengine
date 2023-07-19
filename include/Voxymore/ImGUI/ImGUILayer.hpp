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

    class VXM_API ImGUILayer : public Layer {
    public:
        ImGUILayer();
        ~ImGUILayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        void OnImGuiRender() override;

        ImGuiContext* GetContext() const;

        void Begin();
        void End();
    private:
        float m_Time = 0.0f;
    };

} // Core
