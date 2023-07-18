//
// Created by ianpo on 04/07/2023.
//

#include "VoxymoreCore.hpp"
#include "Application.hpp"


namespace Voxymore::Core {
    Application::Application() {
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application() {

    }
    void Application::OnEvent(Events::Event& e){
        Events::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Events::WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

        VXM_CORE_INFO(e.ToString());

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e);{
                if(e.m_Handled){
                    break;
                }
            }
        }
    }

    void Application::Run() {
        while (m_Running)
        {
            for (Layers::Layer* layer : m_LayerStack) {
                layer->OnUpdate();
            }

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(Events::WindowCloseEvent &e) {
        m_Running = false;
        return true;
    }

    void Application::PushLayer(Layers::Layer *layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layers::Layer *overlay) {
        m_LayerStack.PushOverlay(overlay);
    }
} // Core