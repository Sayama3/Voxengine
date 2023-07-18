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

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline static Application& Get() {return *s_Instance; }
        inline Window& GetWindow() { return *m_Window; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
        static Application* s_Instance;
    };

} // Core
