//
// Created by ianpo on 04/07/2023.
//

#include "Application.hpp"

namespace Voxymore::Core {
    Application::Application() {
        m_Window = std::unique_ptr<Window>(Window::Create());
    }

    Application::~Application() {

    }

    void Application::Run() {
        while (m_Running)
        {
            m_Window->OnUpdate();
        }
    }
} // Core