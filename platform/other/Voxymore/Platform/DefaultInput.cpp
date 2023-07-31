//
// Created by ianpo on 18/07/2023.
//

#include "DefaultInput.hpp"
#include "Voxymore/Core/Application.hpp"
#include "Voxymore/Core/Core.hpp"
#include "GLFW/glfw3.h"

namespace Voxymore::Core {
    Input* Input::s_Instance = new DefaultInput();

    bool DefaultInput::IsKeyPressedImpl(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto keyState = glfwGetKey(window, keycode);
        return keyState == GLFW_PRESS || keyState == GLFW_REPEAT;
    }

    bool DefaultInput::IsMouseButtonPressedImpl(int button) {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto buttonState = glfwGetMouseButton(window, button);
        return buttonState == GLFW_PRESS;
    }

    std::pair<float, float> DefaultInput::GetMousePositionImpl() {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return {static_cast<float>(x),static_cast<float>(y)};
    }

    float DefaultInput::GetMouseXImpl() {
        auto[x, y] = GetMousePositionImpl();
        return static_cast<float>(x);
    }

    float DefaultInput::GetMouseYImpl() {
        auto[x, y] = GetMousePositionImpl();
        return static_cast<float>(y);
    }
} // Core