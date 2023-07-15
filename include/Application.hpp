//
// Created by ianpo on 04/07/2023.
//

#pragma once

#include "Window.hpp"

namespace Voxymore::Core {

    class VXM_API Application {
    public:
        Application();
        virtual ~Application();
        void Run();
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

} // Core
