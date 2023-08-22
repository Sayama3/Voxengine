//
// Created by ianpo on 04/07/2023.
//

#pragma once

#include "Core.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Events/Event.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"
#include "Voxymore/Layers/LayerStack.hpp"
#include "Voxymore/ImGUI/ImGUILayer.hpp"
#include "Window.hpp"

namespace Voxymore::Core {

    class Application {
    public:
        Application(const std::string& name = "Voxymore Application");
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline static Application& Get() {return *s_Instance; }
        inline Window& GetWindow() { return *m_Window; }

        void Close();

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
    private:
        double m_LastFrameTime = 0.0;
        Scope<Window> m_Window;
        ImGUILayer* m_ImGUILayer;
        bool m_Running = true;
        LayerStack m_LayerStack;
        static Application* s_Instance;
    };

} // Core
