//
// Created by ianpo on 04/07/2023.
//

#pragma once

#include "Events/Event.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Window.hpp"

namespace Voxymore::Core {

    class VXM_API Application {
    public:
        Application();
        virtual ~Application();
        void Run();
        void OnEvent(Events::Event& e);
    private:
        bool OnWindowClose(Events::WindowCloseEvent& e);
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

} // Core
