//
// Created by ianpo on 19/07/2023.
//

#include "Voxymore/Renderer/OpenGLContext.hpp"
#include "Voxymore/Logger.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Voxymore {
    namespace Core {
        OpenGLContext::OpenGLContext(GLFWwindow *windowHandle) : m_WindowHandle(windowHandle) {

        }

        OpenGLContext::~OpenGLContext() {

        }

        void OpenGLContext::Init() {
            glfwMakeContextCurrent(m_WindowHandle);
            int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            VXM_CORE_INFO("glad loading status: {0}", status);

        }

        void OpenGLContext::SwapBuffers() {
            glfwSwapBuffers(m_WindowHandle);
        }
    } // Voxymore
} // Core