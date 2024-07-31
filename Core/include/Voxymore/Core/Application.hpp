//
// Created by ianpo on 04/07/2023.
//

#pragma once

#include "Core.hpp"
#include "Voxymore/Core/FileSystem.hpp"
#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Core/TimeStep.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"
#include "Voxymore/Events/Event.hpp"
#include "Voxymore/ImGui/ImGuiLayer.hpp"
#include "Voxymore/Layers/LayerStack.hpp"
#include "Window.hpp"

namespace Voxymore::Core {
	struct ApplicationParameters
	{
		std::string name = "Voxymore Application";
		uint32_t width = 1600;
		uint32_t height = 900;
		std::vector<std::string> arguments;
		bool addImGuiLayer = true;
		bool addParticlePhysicsLayer = true;
		bool addRigidbodyPhysicsLayer = true;
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
		inline const ApplicationParameters& GetParameters() const {return m_Parameters;}

		const std::string& GetArgument(const std::string& key) const;
		const std::string& GetArgument(int key) const;
		bool HasArgument(int key) const;

		void Close();

		template<typename T>
		T* FindLayer();

		template<typename T>
		const T* FindLayer() const;

		template<typename T>
		bool TryGetLayer(T*& ptr);

		template<typename T>
		const bool TryGetLayer(const T*& ptr) const;
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		void ProcessArguments(const std::vector<std::string>& arguments);
	private:
		ApplicationParameters m_Parameters;
		double m_LastFrameTime = 0.0;
		Scope<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		static Application* s_Instance;
	};

	template<typename T>
	inline T* Application::FindLayer()
	{
		VXM_PROFILE_FUNCTION();
		return m_LayerStack.GetLayer<T>();
	}

	template<typename T>
	inline const T* Application::FindLayer() const
	{
		VXM_PROFILE_FUNCTION();
		return m_LayerStack.GetLayer<T>();
	}

	template<typename T>
	inline bool Application::TryGetLayer(T*& ptr)
	{
		VXM_PROFILE_FUNCTION();
		return m_LayerStack.TryGetLayer(ptr);
	}

	template<typename T>
	inline const bool Application::TryGetLayer(const T*& ptr) const
	{
		VXM_PROFILE_FUNCTION();
		return m_LayerStack.TryGetLayer(ptr);
	}

} // Core
