//
// Created by ianpo on 19/07/2023.
//

#pragma once
#include "Voxymore/Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Voxymore::Core {

    class OpenGLContext : public GraphicsContext {
    public:
        OpenGLContext(GLFWwindow* windowHandle);
        virtual ~OpenGLContext();
        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        GLFWwindow* m_WindowHandle;
    };

} // Core
