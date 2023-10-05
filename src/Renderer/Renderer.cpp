//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/OpenGL/OpenGLShader.hpp"

namespace Voxymore::Core {
	static RendererData s_Data;

	void Renderer::Init() {
		VXM_PROFILE_FUNCTION();
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);
		s_Data.ModelUniformBuffer = UniformBuffer::Create(sizeof(RendererData::ModelData), 1);

		RenderCommand::Init();
	}

	void Renderer::Shutdown() {
		VXM_PROFILE_FUNCTION();

		RenderCommand::Shutdown();
	}

	void Renderer::BeginScene(const PerspectiveCamera& camera) {
		VXM_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
	}

	void Renderer::BeginScene(const EditorCamera &camera)
	{
		VXM_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjectionMatrix = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
	}

	void Renderer::BeginScene(const Camera &camera, const glm::mat4 &transform)
	{
		VXM_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjectionMatrix = camera.GetProjectionMatrix() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
	}

	void Renderer::EndScene() {
		VXM_PROFILE_FUNCTION();
	}

	void Renderer::Submit(Ref<Shader>& shader, const Ref<VertexArray> &vertexArray, const glm::mat4& transform, int entityId) {
		VXM_PROFILE_FUNCTION();
		//        VXM_CORE_ASSERT(s_Data.CameraBuffer.ViewProjectionMatrix != glm::zero<glm::mat4>(), "A valid View Projection Matrix is required to submit data to the renderer.");


		s_Data.ModelBuffer.TransformMatrix = transform;
		s_Data.ModelBuffer.EntityId = entityId;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		shader->Bind();
		//TODO: Set the view projection matrix once per frame not once per model drawn.
		//        std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_ViewProjectionMatrix", s_Data.ViewProjectionMatrix);
		//        std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	void Renderer::Submit(Ref<Material> &material, const Ref<VertexArray> &vertexArray, const glm::mat4 &transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		//		VXM_CORE_ASSERT(s_Data.ViewProjectionMatrix != glm::zero<glm::mat4>(), "A valid View Projection Matrix is required to submit data to the renderer.");


		s_Data.ModelBuffer.TransformMatrix = transform;
		s_Data.ModelBuffer.EntityId = entityId;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		//		material->SetUniformMat4("u_Projection.ViewProjectionMatrix", s_Data.ViewProjectionMatrix);
		//		material->SetUniformMat4("u_Projection.Transform", transform);
		material->Bind();
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		VXM_PROFILE_FUNCTION();
		RenderCommand::SetViewport(0,0,width,height);
	}
} // Core