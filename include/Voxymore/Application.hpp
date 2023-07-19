//
// Created by ianpo on 04/07/2023.
//

#pragma once

#include "Voxymore/Core.hpp"
#include "Voxymore/Events/Event.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"
#include "Voxymore/Layers/LayerStack.hpp"
#include "Voxymore/ImGUI/ImGUILayer.hpp"
#include "Window.hpp"

namespace Voxymore::Core {

    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline static Application& Get() {return *s_Instance; }
        inline Window& GetWindow() { return *m_Window; }
        inline ImGUILayer& GetImGUILayer() {return *m_ImGUILayer;}
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        std::unique_ptr<Window> m_Window;
        ImGUILayer* m_ImGUILayer;
        bool m_Running = true;
        LayerStack m_LayerStack;
        static Application* s_Instance;
    };

} // Core
