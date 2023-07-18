//
// Created by ianpo on 18/07/2023.
//

#pragma once

#include "Input.hpp"

namespace Voxymore::Core {
    class DefaultInput : public Input
    {
    protected:
        virtual bool IsKeyPressedImpl(int keycode) override;
        virtual bool IsMouseButtonPressedImpl(int button) override;
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
    };

} // Core
