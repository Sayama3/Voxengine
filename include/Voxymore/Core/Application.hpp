//
// Created by ianpo on 04/07/2023.
//

#pragma once

#include "Core.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Events/Event.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"
#include "Voxymore/Layers/LayerStack.hpp"
#include "Voxymore/ImGUI/ImGUILayer.hpp"
#include "Window.hpp"

namespace Voxymore::Core {
	struct ApplicationParameters
	{
		std::string name = "Voxymore Application";
		uint32_t width = 1600;
		uint32_t height = 900;
		std::vector<std::string> arguments;
	};

	class Application {
	public:
		Application(ApplicationParameters parameters);
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() {return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		inline ImGUILayer* GetImGuiLayer() { return m_ImGUILayer; }
		inline const ApplicationParameters& GetParameters() const {return m_Parameters;}

		const std::string& GetArgument(const std::string& key) const;
		const std::string& GetArgument(int key) const;
		bool HasArgument(int key) const;

		void Close();
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		void ProcessArguments(const std::vector<std::string>& arguments);
	private:
		ApplicationParameters m_Parameters;
		double m_LastFrameTime = 0.0;
		Scope<Window> m_Window;
		ImGUILayer* m_ImGUILayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		static Application* s_Instance;
	};

} // Core
