//
// Created by ianpo on 04/07/2023.
//

#include "Voxymore/Application.hpp"
#include "Voxymore/Macros.hpp"
#include "Voxymore/Logger.hpp"
#include "Voxymore/Core.hpp"

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

        float vertices [3 * 3] = {
                -0.5f, -0.5f, -0.5f, // 0
                0.0f, 0.5f, 0.0f,    // 1
                0.5f, -0.5f, 0.5f,   // 2
        };
        m_VertexBuffer.reset(VertexBuffer::Create(sizeof(vertices), vertices));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        uint32_t index[3] = {
                0,
                2,
                1,
        };

        m_IndexBuffer.reset(IndexBuffer::Create(std::size(index), index));

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            void main() {
                gl_Position = vec4(a_Position, 1.0);
            }
            )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 o_Color;

            void main() {
                o_Color = vec4(0.8, 0.2, 0.3, 1.0);
            }
        )";
        m_Shader.reset(Shader::CreateShader({vertexSrc, ShaderType::VERTEX_SHADER}, {fragmentSrc, ShaderType::FRAGMENT_SHADER}));
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

            m_Shader->Bind();
//            m_ShaderProgram->Bind();

            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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