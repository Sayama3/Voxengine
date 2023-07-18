//
// Created by ianpo on 14/07/2023.
//

#include "Window.hpp"
#include "DefaultWindow.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/KeyEvent.hpp"


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
        glfwMakeContextCurrent(m_Window);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        VXM_CORE_INFO("glad loading status: {0}", status);

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW Callbacks

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            Events::WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);

            Events::WindowCloseEvent event;
            data.EventCallback(event);
        });
//        Parameters
//        window - The window that received the event.
//        key - The [keyboard key](Ref keys) that was pressed or released.
//        scancode - The platform-specific scancode of the key.
//        action - `GLFW_PRESS`, `GLFW_RELEASE` or `GLFW_REPEAT`. Future releases may add more actions.
//        mods - Bit field describing which [modifier keys](mods) were held down
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);

            switch ((GLFWKeyState)action) {
                case GLFWKeyState::PRESS:
                {
                    Events::KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFWKeyState::RELEASE:
                {
                    Events::KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFWKeyState::REPEAT:
                {
                    // Can be extract if we want the number of repeat/
                    Events::KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);

            switch ((GLFWKeyState)action) {
                case GLFWKeyState::PRESS:
                {
                    Events::MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFWKeyState::RELEASE:
                {
                    Events::MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset){
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);

            Events::MouseScrolledEvent event(static_cast<float>(xoffset), static_cast<float>(yoffset));
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos){
            DefaultData& data = *(DefaultData*)glfwGetWindowUserPointer(window);

            Events::MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
            data.EventCallback(event);
        });

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