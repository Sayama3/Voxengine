//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core.hpp"
#include "RenderCommand.hpp"

namespace Voxymore {
    namespace Core {

        class Renderer {
        public:
            static void BeginScene();
            static void EndScene();

            static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

            inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
        };

    } // Voxymore
} // Core
