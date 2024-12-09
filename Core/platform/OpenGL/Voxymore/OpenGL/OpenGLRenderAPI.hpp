//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Renderer/RendererAPI.hpp"

namespace Voxymore::Core {

        class OpenGLRenderAPI : public RendererAPI {
        public:
            virtual void Init() override;
            virtual void Shutdown() override;
            virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
            virtual void SetClearColor(const glm::vec4& color) override;
            virtual void Clear() override;
			virtual void ClearVABinding() override;
			virtual void ClearShaderBinding() override;
			virtual void UnbindTexture(uint32_t slot) override;

            virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
			virtual void DrawIndexed(DrawMode drawMode, const Ref<VertexArray>& vertexArray) override;

			// TODO? add a starting index
			virtual void DrawPatches(uint32_t verticesPerPatch) override;
			virtual void GPUDraw(uint32_t count, uint32_t offset, DrawMode drawMode) override;
			virtual void DrawCubemap(Ref<Cubemap> cubemap, Ref<Shader> shader, Ref<VertexArray> mesh) override;
			void SetPatchSize(int32_t verticesPerPatch);
			virtual void DisableDepth() override;
			virtual void EnableDepth() override;
			virtual void DisableWireframe() override;
			virtual void EnableWireframe() override;
			virtual void EnableDoubleSided() override;
			virtual void DisableDoubleSided() override;
        };

    } // Core
