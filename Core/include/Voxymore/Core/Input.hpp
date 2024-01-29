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
        static bool IsKeyPressed(Key keycode);
        static bool IsMouseButtonPressed(Mouse button);
        static float GetMouseX();
        static float GetMouseY();
        static std::pair<float, float> GetMousePosition();
    };
}