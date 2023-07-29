//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Renderer/RendererAPI.hpp"

namespace Voxymore::Core {

        class OpenGLRenderAPI : public RendererAPI {
        public:
            OpenGLRenderAPI();
            virtual ~OpenGLRenderAPI() override;
            virtual void Init() override;
            virtual void SetClearColor(const glm::vec4& color) override;
            virtual void Clear() override;

            virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
        };

    } // Core
