//
// Created by ianpo on 18/07/2023.
//


#include <Voxymore/ImGUI/ImGUILayer.hpp>
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Logger.hpp"
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
    ImGUILayer::ImGUILayer() : Layer("ImGUILayer") {
		VXM_PROFILE_FUNCTION();
        VXM_CORE_INFO("Create ImGUILayer");
		m_Fonts.reserve((uint8_t )FontType::Count);
    }

    ImGUILayer::~ImGUILayer() {
        VXM_CORE_INFO("Destroy ImGUILayer");
    }

    void ImGUILayer::OnAttach() {
        VXM_PROFILE_FUNCTION();

        VXM_CORE_INFO("Attach ImGUILayer");

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

    void ImGUILayer::OnDetach() {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_INFO("Detach ImGUILayer");
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGUILayer::Begin() {
        VXM_PROFILE_FUNCTION();
//        VXM_CORE_INFO("Begin ImGUILayer");
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		ImGuizmo::BeginFrame();
//        VXM_CORE_INFO("Add every new Frames");
    }

    void ImGUILayer::End() {
        VXM_PROFILE_FUNCTION();
//        VXM_CORE_INFO("End ImGUILayer");

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

    void ImGUILayer::OnImGuiRender() {
        VXM_PROFILE_FUNCTION();
//        VXM_CORE_INFO("ImGUILayer -> OnImGuiRender");
//        static bool showDemoWindow = true;
//        if(showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);
    }

    void ImGUILayer::OnEvent(Event &e) {
		VXM_PROFILE_FUNCTION();
        if(m_BlockEvents) {
            ImGuiIO &io = ImGui::GetIO();
            e.m_Handled |= e.IsInCategory(EventCategory::EventCategoryMouse) & io.WantCaptureMouse;
            e.m_Handled |= e.IsInCategory(EventCategory::EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

	void ImGUILayer::AddFont(const std::string &path, float size, FontType fontType, bool isDefault)
	{
		VXM_PROFILE_FUNCTION();
		ImGuiIO &io = ImGui::GetIO();
		auto* fontPtr = io.Fonts->AddFontFromFileTTF(path.c_str(), size);
		if(isDefault) io.FontDefault = fontPtr;
		m_Fonts[fontType] = fontPtr;
	}

	ImFont* ImGUILayer::GetFont(FontType fontType)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_Fonts.contains(fontType), "The ImGUILayer doesn't not contain the FontType {0}", static_cast<uint8_t>(fontType));
		return m_Fonts[fontType];
	}

	void ImGUILayer::PushFont(FontType fontType)
	{
		VXM_PROFILE_FUNCTION();
		VXM_CORE_ASSERT(m_Fonts.contains(fontType), "The ImGUILayer doesn't not contain the FontType {0}", static_cast<uint8_t>(fontType));
		ImGui::PushFont(m_Fonts[fontType]);
	}
	void ImGUILayer::SetDarkThemeColors()
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
	void ImGUILayer::SetLightThemeColors()
	{
		VXM_PROFILE_FUNCTION();
		m_IsDarkTheme = false;
		ImGui::StyleColorsLight();
		ImGuiStyle& style = ImGui::GetStyle();
		auto& colors = style.Colors;
		//TODO: Set the colors lights.
	}
} // Core