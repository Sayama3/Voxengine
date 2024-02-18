//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/OpenGL/OpenGLShader.hpp"

namespace Voxymore::Core {
	static RendererData s_Data;
	static std::string s_BindedShader;

	void Renderer::Init() {
		VXM_PROFILE_FUNCTION();
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);
		s_Data.ModelUniformBuffer = UniformBuffer::Create(sizeof(RendererData::ModelData), 1);
		s_Data.LightUniformBuffer = UniformBuffer::Create(sizeof(RendererData::LightData), 2);
		s_Data.MaterialUniformBuffer = UniformBuffer::Create(sizeof(MaterialParameters), 3);

		RenderCommand::Init();
	}

	void Renderer::Shutdown() {
		VXM_PROFILE_FUNCTION();

		RenderCommand::Shutdown();
	}

	void Renderer::BeginScene(const EditorCamera &camera, std::vector<Light> lights)
	{
		VXM_PROFILE_FUNCTION();

		s_BindedShader = "";
		s_Data.CameraBuffer.ViewProjectionMatrix = camera.GetViewProjection();
		s_Data.CameraBuffer.CameraPosition = glm::vec4(camera.GetPosition(), 1);
		s_Data.CameraBuffer.CameraDirection = glm::vec4(camera.GetForwardDirection(), 0);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
		s_Data.LightBuffer.lightCount = std::min((int)lights.size(), MAX_LIGHT_COUNT);
		for (size_t i = 0; i < s_Data.LightBuffer.lightCount; ++i)
		{
			s_Data.LightBuffer.lights[i] = lights[i];
		}
		s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer, sizeof(RendererData::LightData));
	}

	void Renderer::BeginScene(const Camera &camera, const glm::mat4 &transform, std::vector<Light> lights)
	{
		VXM_PROFILE_FUNCTION();
		s_Data.CameraBuffer.ViewProjectionMatrix = camera.GetProjectionMatrix() * glm::inverse(transform);
		auto p = transform * glm::vec4{0,0,0,1};
		s_Data.CameraBuffer.CameraPosition = glm::vec4(glm::vec3(p) / p.w, 1);
		s_Data.CameraBuffer.CameraDirection = transform * glm::vec4{0,0,1,0};
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
		s_Data.LightBuffer.lightCount = std::min((int)lights.size(), MAX_LIGHT_COUNT);
		for (size_t i = 0; i < s_Data.LightBuffer.lightCount; ++i)
		{
			s_Data.LightBuffer.lights[i] = lights[i];
		}
		s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer, sizeof(RendererData::LightData));
	}

	void Renderer::EndScene() {
		VXM_PROFILE_FUNCTION();
		RenderCommand::ClearBinding();
		s_BindedShader = "";
	}

	void Renderer::Submit(Ref<Shader>& shader, const Ref<VertexArray> &vertexArray, const glm::mat4& transform, int entityId) {
		VXM_PROFILE_FUNCTION();
		//        VXM_CORE_ASSERT(s_Data.CameraBuffer.ViewProjectionMatrix != glm::zero<glm::mat4>(), "A valid View Projection Matrix is required to submit data to the renderer.");


		s_Data.ModelBuffer.TransformMatrix = transform;
		s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ModelBuffer.EntityId = entityId;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		auto& shaderName = shader->GetName();
		if(shaderName != s_BindedShader)
		{
			shader->Bind();
			s_BindedShader = shaderName;
		}
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
		s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ModelBuffer.EntityId = entityId;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		s_Data.MaterialUniformBuffer->SetData(&material->GetMaterialsParameters(), sizeof(MaterialParameters));
		auto& shaderName = material->GetShaderName();
		if(shaderName != s_BindedShader)
		{
			material->Bind();
			s_BindedShader = shaderName;
		}
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const MeshGroup& meshGroup, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		s_Data.ModelBuffer.TransformMatrix = transform;
		s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ModelBuffer.EntityId = entityId;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		for (const auto& mesh : meshGroup.GetSubMeshes())
		{
			s_Data.MaterialUniformBuffer->SetData(&mesh.GetMaterial()->GetMaterialsParameters(), sizeof(MaterialParameters));
			mesh.Bind();
			auto& shaderName = mesh.GetMaterial()->GetShaderName();
			if(shaderName != s_BindedShader)
			{
				mesh.GetMaterial()->Bind();
				s_BindedShader = shaderName;
			}
			RenderCommand::DrawIndexed(mesh.GetVertexArray());
		}
	}

	void Renderer::Submit(const Ref<Mesh>& mesh, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		Submit(*mesh, transform, entityId);
	}

	void Renderer::Submit(const Mesh& mesh, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		s_Data.ModelBuffer.TransformMatrix = transform;
		s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ModelBuffer.EntityId = entityId;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));
		s_Data.MaterialUniformBuffer->SetData(&mesh.GetMaterial()->GetMaterialsParameters(), sizeof(MaterialParameters));
		mesh.Bind();
		auto& shaderName = mesh.GetMaterial()->GetShaderName();
		if(shaderName != s_BindedShader)
		{
			mesh.GetMaterial()->Bind();
			s_BindedShader = shaderName;
		}
		RenderCommand::DrawIndexed(mesh.GetVertexArray());
	}

	void Renderer::Submit(const Ref<Model>& model, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		model->Bind();
		for (int nodeIndex : model->GetDefaultScene())
		{
			Submit(model, model->GetNode(nodeIndex), transform, entityId);
		}
	}

	void Renderer::Submit(const Ref<Model>& model, const Node& node, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		glm::mat4 currentTransform = transform * node.transform;
		if(node.HasMesh())
		{
			Submit(model->GetMeshGroup(node.GetMeshIndex()), currentTransform, entityId);
		}

		if(node.HasChildren())
		{
			for(const int i : node.children)
			{
				Submit(model, model->GetNode(i), currentTransform, entityId);
			}
		}
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		VXM_PROFILE_FUNCTION();
		RenderCommand::SetViewport(0,0,width,height);
	}
} // Core