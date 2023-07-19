//
// Created by ianpo on 19/07/2023.
//

#pragma once

#include "Voxymore/Core.hpp"

namespace Voxymore::Core{
    class GraphicsContext{
    public:

        virtual ~GraphicsContext() {}
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}
