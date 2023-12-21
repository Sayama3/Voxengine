//
// Created by ianpo on 18/07/2023.
//

#include "Voxymore/Core/Input.hpp"
#include "Voxymore/Core/Application.hpp"
#include "Voxymore/Core/Core.hpp"
#include "GLFW/glfw3.h"

namespace Voxymore::Core {
    bool Input::IsKeyPressed(Key keycode)
    {
        VXM_PROFILE_FUNCTION();
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto keyState = glfwGetKey(window, static_cast<int32_t>(keycode));
        return keyState == GLFW_PRESS || keyState == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(Mouse button)
    {
        VXM_PROFILE_FUNCTION();
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto buttonState = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return buttonState == GLFW_PRESS;
    }

    std::pair<float, float> Input::GetMousePosition() {
        VXM_PROFILE_FUNCTION();
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return {static_cast<float>(x),static_cast<float>(y)};
    }

    float Input::GetMouseX() {
		VXM_PROFILE_FUNCTION();
        auto[x, y] = GetMousePosition();
        return x;
    }

    float Input::GetMouseY() {
		VXM_PROFILE_FUNCTION();
        auto[x, y] = GetMousePosition();
        return y;
    }
} // Core