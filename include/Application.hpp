//
// Created by ianpo on 04/07/2023.
//

#pragma once

#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Layers/LayerStack.hpp"
#include "Window.hpp"

namespace Voxymore::Core {

    class VXM_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Events::Event& e);

        void PushLayer(Layers::Layer* layer);
        void PushOverlay(Layers::Layer* overlay);
    private:
        bool OnWindowClose(Events::WindowCloseEvent& e);
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        Layers::LayerStack m_LayerStack;
    };

} // Core
