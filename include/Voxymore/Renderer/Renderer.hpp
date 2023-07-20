//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core.hpp"

namespace Voxymore {
    namespace Core {
        enum class RendererAPI {
            None = 0,
            OpenGL = 1,
        };

        std::string RendererAPIToString(RendererAPI api){
            switch (api) {

                case RendererAPI::None:
                    return "RendererAPI::None";
                case RendererAPI::OpenGL:
                    return "RendererAPI::OpenGL";
            }
            return "Unknown";
        }

        inline std::ostream& operator<<(std::ostream& os, const RendererAPI& api)
        {
            return os << RendererAPIToString(api);
        }

        class Renderer {
        public:
            inline static RendererAPI GetAPI()  { return s_RendererAPI; }
            static void SetAPI(RendererAPI api);
        private:
            static RendererAPI s_RendererAPI;
        };

    } // Voxymore
} // Core
