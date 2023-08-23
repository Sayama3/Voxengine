//
// Created by ianpo on 18/07/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/KeyCodes.hpp"
#include "Voxymore/Core/MouseButtonCodes.hpp"

namespace Voxymore::Core {
    class Input {
    public:
        static bool IsKeyPressed(KeyCode keycode);
        static bool IsMouseButtonPressed(MouseButton button);
        static float GetMouseX();
        static float GetMouseY();
        static std::pair<float, float> GetMousePosition();
    };
}