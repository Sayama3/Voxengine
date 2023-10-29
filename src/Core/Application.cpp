//
// Created by ianpo on 04/07/2023.
//

#include <utility>

#include "Voxymore/Core/Application.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Utils/Platform.hpp"


namespace Voxymore::Core {

    Application* Application::s_Instance = nullptr;
    Application::Application(ApplicationParameters  parameters) : m_Parameters(std::move(parameters)) {
        VXM_PROFILE_FUNCTION();
        if(s_Instance != nullptr){
            VXM_CORE_ERROR("There should only be one application.");
        }

        s_Instance = this;

		if(m_Parameters.arguments.size() > 0)
		{
			ProcessArguments(m_Parameters.arguments);
		}

        WindowProps props(m_Parameters.name, m_Parameters.width, m_Parameters.height);
        m_Window = Scope<Window>(Window::Create(props));
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        m_ImGUILayer = new ImGUILayer();
        PushOverlay(m_ImGUILayer);
    }

    Application::~Application() {

    }
    void Application::OnEvent(Event& e){
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

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

	void Application::ProcessArguments(const std::vector<std::string>& arguments)
	{
		size_t argc = arguments.size();
		std::filesystem::path exePath = arguments[0];
		FileSystem::s_EditorPath = exePath.parent_path();
		for (int i = 1; i < arguments.size(); ++i)
		{
			if(arguments[i]== "--root" || arguments[i] == "-r")
			{
				VXM_CORE_ASSERT((i+1) < argc, "Not enough arguments.");
				FileSystem::s_EditorPath = arguments[++i];
			}
		}
	}

	const std::string& Application::GetArgument(const std::string& key) const
	{
		const std::vector<std::string>& arguments = m_Parameters.arguments;
		size_t argc = arguments.size();
		for (int i = 1; i < arguments.size(); ++i)
		{
			if(arguments[i] == key)
			{
				VXM_CORE_ASSERT((i+1) < argc, "Not enough arguments.");
				return arguments[++i];
			}
		}
		return "";
	}

	const std::string& Application::GetArgument(int key) const
	{
		const std::vector<std::string>& arguments = m_Parameters.arguments;
		VXM_CORE_ASSERT(key >= 0 && key < arguments.size(), "The index is not valid.")
		return arguments[key];
	}
} // Core