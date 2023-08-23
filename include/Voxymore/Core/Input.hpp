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
        inline static bool IsKeyPressed(KeyCode keycode);
        inline static bool IsMouseButtonPressed(MouseButton button);
        inline static float GetMouseX();
        inline static float GetMouseY();
        inline static std::pair<float, float> GetMousePosition();
    };
}