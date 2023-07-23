//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Logger.hpp"

namespace Voxymore::Core {
    void Renderer::BeginScene(const Camera& camera) {
		s_Camera = &camera;
    }

    void Renderer::EndScene() {
		s_Camera = nullptr;
    }

    void Renderer::Submit(std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray> &vertexArray) {
		if(s_Camera == nullptr) {
			VXM_CORE_ERROR("No valid camera set, begin the scene with a camera before submitting meshes.");
			return;
		}
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjectionMatrix", s_Camera->GetViewProjectionMatrix());
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
} // Core