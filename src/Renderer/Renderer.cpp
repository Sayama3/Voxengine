//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/Renderer.hpp"

namespace Voxymore::Core {
    void Renderer::BeginScene() {

    }

    void Renderer::EndScene() {

    }

    void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray) {
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
} // Core