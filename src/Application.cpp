//
// Created by ianpo on 04/07/2023.
//

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Application.hpp"
#include "Voxymore/Macros.hpp"
#include "Voxymore/Logger.hpp"
#include "Voxymore/Core.hpp"


namespace Voxymore::Core {

    Application* Application::s_Instance = nullptr;
    Application::Application() : m_Camera(800, 600) {

        if(s_Instance != nullptr){
            VXM_CORE_ERROR("There should only be one application.");
        }

        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent, std::placeholders::_1));

        m_ImGUILayer = new ImGUILayer();
        PushOverlay(m_ImGUILayer);

		m_Camera.SetSize(m_Window->GetWidth(), m_Window->GetHeight());

        m_VertexArray.reset(VertexArray::Create());

        float vertices [(3 * 3) + (3 * 3) + (3 * 4)] = {
                -0.5f, -0.5f, -0.5f,    -0.5f, -0.5f, -0.5f,    -0.5f, -0.5f, -0.5f, 1.0f,
                0.0f, 0.5f, 0.0f,       0.0f, 0.5f, 0.0f,       0.0f, 0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f,      0.5f, -0.5f, 0.5f,      0.5f, -0.5f, 0.5f, 1.0f,
        };
        m_VertexBuffer.reset(VertexBuffer::Create(sizeof(vertices), vertices));
        BufferLayout layout = {
                {ShaderDataType::Float3, "a_Position"},
                {ShaderDataType::Float3, "a_Normal"},
                {ShaderDataType::Float4, "a_Color"},
        };
        m_VertexBuffer->SetLayout(layout);


        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        uint32_t index[3] = {
                0,
                2,
                1,
        };

        m_IndexBuffer.reset(IndexBuffer::Create(std::size(index), index));

        m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec3 a_Normal;
            layout(location = 2) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;

            void main() {
                gl_Position = vec4(a_Position, 1.0);
                v_Position = a_Position;
                v_Color = a_Color;
            }
            )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 o_Color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main() {
                o_Color = v_Color;
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
            RenderCommand::SetClearColor({0.1f,0.1f,0.1f,1});
            RenderCommand::Clear();

            Renderer::BeginScene(m_Camera);

            Renderer::Submit(m_Shader, m_VertexArray);

            Renderer::EndScene();

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