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

        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);

        glGenBuffers(1, &m_VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

        float vertices [3 * 3] = {
                -0.5f, -0.5f, -0.5f, // 0
                0.0f, 0.5f, 0.0f,    // 1
                0.5f, -0.5f, 0.5f,   // 2
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) , vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glGenBuffers(1, &m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
        unsigned int index[3] = {
                0,
                2,
                1,
        };
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index) , index, GL_STATIC_DRAW);
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
            VXM_CORE_INFO("Clear Screen");
            // Softer color.
            glClearColor(0.1f,0.1f,0.1f,1);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

            for (Layer* layer : m_LayerStack) {
                VXM_CORE_INFO("Update Layer {0}", layer->GetName());
                layer->OnUpdate();
            }

            VXM_CORE_INFO("m_ImGUILayer->Begin(): {0}", m_ImGUILayer->GetName());
            m_ImGUILayer->Begin();

            for (Layer* layer : m_LayerStack) {
                VXM_CORE_INFO("OnImGuiRender: {0}", layer->GetName());
                layer->OnImGuiRender();
            }
            VXM_CORE_INFO("m_ImGUILayer->End: {0}", m_ImGUILayer->GetName());
            m_ImGUILayer->End();

            VXM_CORE_INFO("Window Update");
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