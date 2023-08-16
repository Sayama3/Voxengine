//
// Created by ianpo on 25/07/2023.
//

#include "Voxymore/Core/Platform.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#if VXM_DEFAULT
#include <GLFW/glfw3.h>
#endif

namespace Voxymore::Core{
    double Platform::GetTime() {
#if VXM_DEFAULT
        return glfwGetTime();
#endif
    }
}