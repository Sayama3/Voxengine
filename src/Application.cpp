//
// Created by ianpo on 04/07/2023.
//

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Application.hpp"
#include "Voxymore/Macros.hpp"
#include "Voxymore/Logger.hpp"
#include "Voxymore/Core.hpp"
#include "Voxymore/Core/Platform.hpp"


namespace Voxymore::Core {

    Application* Application::s_Instance = nullptr;
    Application::Application() {

        if(s_Instance != nullptr){
            VXM_CORE_ERROR("There should only be one application.");
        }

        s_Instance = this;

        m_Window = Scope<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent, std::placeholders::_1));

        m_ImGUILayer = new ImGUILayer();
        PushOverlay(m_ImGUILayer);
    }

    Application::~Application() {

    }
    void Application::OnEvent(Event& e){
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose, std::placeholders::_1));

//        VXM_CORE_INFO(e.ToString());

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
            double time = Platform::GetTime();
            TimeStep timeStep = static_cast<float>(time - m_LastFrameTime);
            m_LastFrameTime = time;
            //TODO: Remove later as it should be abstracted.
//            VXM_CORE_INFO("Clear Screen");
            RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1});
            RenderCommand::Clear();

            for (Layer* layer : m_LayerStack) {
//                VXM_CORE_INFO("Update Layer {0}", layer->GetName());
                layer->OnUpdate(timeStep);
            }

//            VXM_CORE_INFO("m_ImGUILayer->Begin(): {0}", m_ImGUILayer->GetName());
            m_ImGUILayer->Begin();

            for (Layer* layer : m_LayerStack) {
//                VXM_CORE_INFO("OnImGuiRender: {0}", layer->GetName());
                layer->OnImGuiRender();
            }
//            VXM_CORE_INFO("m_ImGUILayer->End: {0}", m_ImGUILayer->GetName());
            m_ImGUILayer->End();

//            VXM_CORE_INFO("Window Update");
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent &e) {
		VXM_CORE_INFO("Closing Voxymore Application.");
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