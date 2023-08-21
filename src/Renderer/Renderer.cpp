//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/OpenGL/OpenGLShader.hpp"

namespace Voxymore::Core {
    Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

    void Renderer::Init() {
        VXM_PROFILE_FUNCTION();

        RenderCommand::Init();
    }

    void Renderer::Shutdown() {
        VXM_PROFILE_FUNCTION();

        RenderCommand::Shutdown();
    }

    void Renderer::BeginScene(const Camera& camera) {
        VXM_PROFILE_FUNCTION();

        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {
        VXM_PROFILE_FUNCTION();
    }

    void Renderer::Submit(Ref<Shader>& shader, const Ref<VertexArray> &vertexArray, const glm::mat4& transform) {
        VXM_PROFILE_FUNCTION();

        VXM_CORE_ASSERT(s_SceneData->ViewProjectionMatrix != glm::zero<glm::mat4>(), "A valid View Projection Matrix is required to submit data to the renderer.");
		shader->Bind();
        //TODO: Set the view projection matrix once per frame not once per model drawn.
        std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_ViewProjectionMatrix", s_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_Transform", transform);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        VXM_PROFILE_FUNCTION();
        RenderCommand::SetViewport(0,0,width,height);
    }
} // Core