//
// Created by ianpo on 14/07/2023.
//

#include "DefaultWindow.hpp"
#include "Voxymore/Core/Window.hpp"
#include "Voxymore/Events/ApplicationEvent.hpp"
#include "Voxymore/Events/MouseEvent.hpp"
#include "Voxymore/Events/KeyEvent.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/OpenGL/OpenGLContext.hpp"
#include "GLFW/glfw3.h"


namespace Voxymore::Core {
    enum GLFWKeyState : int {
        PRESS = GLFW_PRESS,
        RELEASE = GLFW_RELEASE,
        REPEAT = GLFW_REPEAT,
    };

    static bool s_GLFWInitialized = false;
    static void GLFWErrorCallback(int error_code, const char* description){
        VXM_CORE_ERROR("GLFW Error ({0}): {1}", error_code, description);
    }

    Voxymore::Core::Window* Voxymore::Core::Window::Create(const Voxymore::Core::WindowProps &props) {
        return new DefaultWindow(props);
    }

    DefaultWindow::DefaultWindow(const Voxymore::Core::WindowProps &props) : m_CursorState(CursorState::None) {
        Init(props);
    }

    DefaultWindow::~DefaultWindow() {
        Shutdown();
    }

    void DefaultWindow::Init(const Voxymore::Core::WindowProps &props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        m_Data.Minified = false;

        VXM_CORE_INFO("Creating window {0} {1} {2}", props.Title, props.Width, props.Height);

        //TODO: Log Creating window. Logger not done yet.

        if(!s_GLFWInitialized){
            int success = glfwInit();
//            VXM_CORE_ASSERT(success, "GLFW couldn't be initialized.")

            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(
                static_cast<int>(m_Data.Width),
                static_cast<int>(m_Data.Height),
                m_Data.Title.c_str(),
                nullptr,
                nullptr
        );

        m_Context = new OpenGLContext(m_Window);

        m_Context->Init();


        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW Callbacks
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int iconify)
        {
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);
            data.Minified = iconify == GLFW_TRUE;

            WindowMinifyEvent event(iconify == GLFW_TRUE);
            data.EventCallback(event);
;        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);

            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);

            switch ((GLFWKeyState)action) {
                case GLFWKeyState::PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFWKeyState::RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFWKeyState::REPEAT:
                {
                    // Can be extract if we want the number of repeat/
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint){
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(codepoint);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);

            switch ((GLFWKeyState)action) {
                case GLFWKeyState::PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFWKeyState::RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset){
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event(static_cast<float>(xoffset), static_cast<float>(yoffset));
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos){
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
            data.EventCallback(event);
        });

    }

    void DefaultWindow::Shutdown() {
        delete m_Context;
        glfwDestroyWindow(m_Window);
    }

    void DefaultWindow::OnUpdate() {
        glfwPollEvents();
        m_Context->SwapBuffers();
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

    bool DefaultWindow::IsMinify() const {
        return m_Data.Minified || (m_Data.Width == 0 || m_Data.Height == 0);
    }

    void DefaultWindow::SetCursorState(CursorState cursorState) {
        switch (cursorState) {
            case CursorState::None:
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                break;
            case CursorState::Locked:
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                break;
        }
        m_CursorState = cursorState;
    }

} // Core