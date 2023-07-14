//
// Created by ianpo on 14/07/2023.
//

#include "Window.hpp"
#include "DefaultWindow.hpp"

namespace Voxymore::Core {
    static bool s_GLFWInitialized = false;

    Voxymore::Core::Window* Voxymore::Core::Window::Create(const Voxymore::Core::WindowProps &props) {
        return new DefaultWindow(props);
    }

    DefaultWindow::DefaultWindow(const Voxymore::Core::WindowProps &props) {
        Init(props);
    }

    DefaultWindow::~DefaultWindow() {
        Shutdown();
    }

    void DefaultWindow::Init(const Voxymore::Core::WindowProps &props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        //TODO: Log Creating window. Logger not done yet.

        if(!s_GLFWInitialized){
            int success = glfwInit();
//            VXM_CORE_ASSERT(success, "GLFW couldn't be initialized.")

            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(
                static_cast<int>(m_Data.Width),
                static_cast<int>(m_Data.Height),
                m_Data.Title.c_str(),
                nullptr,
                nullptr
        );
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
    }

    void DefaultWindow::Shutdown() {
        glfwDestroyWindow(m_Window);
    }

    void DefaultWindow::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void DefaultWindow::SetVSync(bool enabled) {
        if (enabled) {
            glfwSwapInterval(1);
        }
        else{
            glfwSwapInterval(0);
        }

        m_Data.VSync = enabled;
    }


    bool DefaultWindow::IsVSync() const {
        return m_Data.VSync;
    }

} // Core