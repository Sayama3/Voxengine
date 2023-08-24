//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Renderer/Camera.hpp"
#include "Voxymore/Renderer/PerspectiveCamera.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"
#include "Voxymore/Renderer/RenderCommand.hpp"

namespace Voxymore {
    namespace Core {

        class Renderer {
        public:
            static void Init();
            static void Shutdown();
            static void OnWindowResize(uint32_t width, uint32_t height);

            static void BeginScene(const Camera& camera, const glm::mat4& transform);
            static void BeginScene(const PerspectiveCamera& camera);
            static void EndScene();

            static void Submit(Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
            static void Submit(Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

            inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		private:
            struct SceneData
            {
                glm::mat4 ViewProjectionMatrix;
            };
            static Scope<SceneData> s_SceneData;
        };

    } // Voxymore
} // Core
