//
// Created by ianpo on 04/07/2023.
//

#pragma once

#include <voxymorecore_export.hpp>

namespace Voxymore::Core {

    class VXM_API Application {
    public:
        Application();
        virtual ~Application();
        void Run();
    private:
    };

} // Core
