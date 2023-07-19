//
// Created by ianpo on 04/07/2023.
//

#include "Voxymore/Application.hpp"
#include "Voxymore/Macros.hpp"
#include "Voxymore/Logger.hpp"
#include <Voxymore/Core.hpp>

//TODO: Remove later as it should be abstracted.
#include <glad/glad.h>


namespace Voxymore::Core {
    Application* Application::s_Instance = nullptr;
    Application::Application() {
        if(s_Instance != nullptr){
            VXM_CORE_ERROR("There should only be one application.");
        }

        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent, std::placeholders::_1));

        m_ImGUILayer = new ImGUILayer();
        PushOverlay(m_ImGUILayer);
    }

    Application::~Application() {

    }
    void Application::OnEvent(Event& e){
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose, std::placeholders::_1));

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
            //TODO: Remove later as it should be abstracted.
            glClear(GL_COLOR_BUFFER_BIT);
            // Softer color.
            glClearColor(0.9921568627450980392156862745098,0.73725490196078431372549019607843,0.36862745098039215686274509803922,1);

            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate();
            }
            m_ImGUILayer->Begin();
            for (Layer* layer : m_LayerStack) {
                layer->OnImGuiRender();
            }
            m_ImGUILayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent &e) {
        m_Running = false;
        return true;
    }

    void Application::PushLayer(Layer *layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer *overlay) {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }
} // Core