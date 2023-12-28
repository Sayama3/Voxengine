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

            virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
        };

    } // Core
