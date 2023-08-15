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

namespace Voxymore::Core {
    ImGUILayer::ImGUILayer() : Layer("ImGUILayer") {
        VXM_CORE_INFO("Create ImGUILayer");
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
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

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
        static bool showDemoWindow = true;
        if(showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);
    }
} // Core