//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core.hpp"
#include "RenderCommand.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/Camera.hpp"

namespace Voxymore {
    namespace Core {

        class Renderer {
        public:
            static void BeginScene(const Camera& camera);
            static void EndScene();

            static void Submit(std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

            inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		private:
			static const Camera* s_Camera;
        };

    } // Voxymore
} // Core
