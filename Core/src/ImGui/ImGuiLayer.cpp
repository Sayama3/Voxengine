//
// Created by ianpo on 18/07/2023.
//


#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Logger.hpp"
#include <Voxymore/ImGUI/ImGuiLayer.hpp>
#include <glad/glad.h>

#ifndef IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM 1
#endif

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include "Voxymore/Core/Application.hpp"

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE 1
#endif
#include <GLFW/glfw3.h>
#include <ImGuizmo.h>

namespace Voxymore::Core {
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {
		VXM_PROFILE_FUNCTION();
        VXM_CORE_INFO("Create ImGuiLayer");
		m_Fonts.reserve((uint8_t )FontType::Count);
    }

	ImGuiLayer::~ImGuiLayer() {
        VXM_CORE_INFO("Destroy ImGuiLayer");
    }

    void ImGuiLayer::OnAttach() {
        VXM_PROFILE_FUNCTION();

        VXM_CORE_INFO("Attach ImGuiLayer");

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // IsActive Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // IsActive Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IsActive Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // IsActive Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
//		ImGui::StyleColorsDark();
//		ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

		SetDarkThemeColors();

        // Setup Platform/Renderer backends
        auto& application = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(application.GetWindow().GetNativeWindow());

        // Setup Platform/Renderer bindings.
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach() {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_INFO("Detach ImGuiLayer");
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::Begin() {
        VXM_PROFILE_FUNCTION();
//        VXM_CORE_INFO("Begin ImGuiLayer");
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		ImGuizmo::BeginFrame();
//        VXM_CORE_INFO("Add every new Frames");
    }

    void ImGuiLayer::End() {
        VXM_PROFILE_FUNCTION();
//        VXM_CORE_INFO("End ImGuiLayer");

        // Rendering
        ImGui::Render();

        auto& app = Application::Get();
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::OnImGuiRender() {
        VXM_PROFILE_FUNCTION();
//        VXM_CORE_INFO("ImGuiLayer -> OnImGuiRender");
//        static bool showDemoWindow = true;
//        if(showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);
    }

    void ImGuiLayer::OnEvent(Event &e) {
		VXM_PROFILE_FUNCTION();
        if(m_BlockEvents) {
            ImGuiIO &io = ImGui::GetIO();
            e.m_Handled |= e.IsInCategory(EventCategory::EventCategoryMouse) & io.WantCaptureMouse;
            e.m_Handled |= e.IsInCategory(EventCategory::EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

	void ImGuiLayer::AddFont(const std::string &path, float size, FontType fontType, bool isDefault)
	{
		VXM_PROFILE_FUNCTION();
		ImGuiIO &io = ImGui::GetIO();
		auto* fontPtr = io.Fonts->AddFontFromFileTTF(path.c_str(), size);
		if(isDefault) io.FontDefault = fontPtr;
		m_Fonts[fontType] = fontPtr;
	}

	ImFont*ImGuiLayer::GetFont(FontType fontType)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_Fonts.contains(fontType), "The ImGuiLayer doesn't not contain the FontType {0}", static_cast<uint8_t>(fontType));
		return m_Fonts[fontType];
	}

	void ImGuiLayer::PushFont(FontType fontType)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_Fonts.contains(fontType), "The ImGuiLayer doesn't not contain the FontType {0}", static_cast<uint8_t>(fontType));
		ImGui::PushFont(m_Fonts[fontType]);
	}
	void ImGuiLayer::SetDarkThemeColors()
	{
		VXM_PROFILE_FUNCTION();
		m_IsDarkTheme = true;
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		auto& colors = style.Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}
	void ImGuiLayer::SetLightThemeColors()
	{
		VXM_PROFILE_FUNCTION();
		m_IsDarkTheme = false;
		ImGui::StyleColorsLight();
		ImGuiStyle& style = ImGui::GetStyle();
		auto& colors = style.Colors;
		//TODO: Set the colors lights.
	}
} // Core