//
// Created by ianpo on 04/07/2023.
//

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Core/Application.hpp"
#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Platform.hpp"


namespace Voxymore::Core {

    Application* Application::s_Instance = nullptr;
    Application::Application() {
        VXM_PROFILE_FUNCTION();
        if(s_Instance != nullptr){
            VXM_CORE_ERROR("There should only be one application.");
        }

        s_Instance = this;

        m_Window = Scope<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent, std::placeholders::_1));

        Renderer::Init();

        m_ImGUILayer = new ImGUILayer();
        PushOverlay(m_ImGUILayer);
    }

    Application::~Application() {

    }
    void Application::OnEvent(Event& e){
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose, std::placeholders::_1));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize, std::placeholders::_1));

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
            TimeStep timeStep;
            {
                VXM_PROFILE_SCOPE("Application::Run -> Get TimeStep");
                double time = Platform::GetTime();
                timeStep = static_cast<float>(time - m_LastFrameTime);
                m_LastFrameTime = time;
            }
            //TODO: Remove later as it should be abstracted?
            if(!GetWindow().IsMinify())
            {
                {
                    VXM_PROFILE_SCOPE("Application::Run -> Update Layer");
                    for (Layer *layer: m_LayerStack) {
                        layer->OnUpdate(timeStep);
                    }
                }
            }

            m_ImGUILayer->Begin();
            {
                VXM_PROFILE_SCOPE("Application::Run -> ImGuiRender Layer");
                for (Layer *layer: m_LayerStack) {
                    layer->OnImGuiRender();
                }
            }
            m_ImGUILayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent &e) {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent &e)
    {
        if(e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            return false;
        }

        // Resizing the renderer only if we got a valid width & height.
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }

    void Application::PushLayer(Layer *layer) {
        VXM_PROFILE_FUNCTION();
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer *overlay) {
        VXM_PROFILE_FUNCTION();
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::Close() {
        VXM_CORE_INFO("Application is closing.");
        m_Running = false;
    }
} // Core