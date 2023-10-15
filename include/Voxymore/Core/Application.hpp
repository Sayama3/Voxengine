//
// Created by ianpo on 04/07/2023.
//

#pragma once

#include "Core.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
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
		int argc = 0;
		char** argv = nullptr;
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

		void Close();

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		void ProcessArguments(int argc, char** argv);
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
