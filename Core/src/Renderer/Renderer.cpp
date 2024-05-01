//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/OpenGL/OpenGLShader.hpp"

namespace Voxymore::Core {
	static RendererData s_Data;
	static ShaderField s_BindedShader = NullAssetHandle;
	static MaterialField s_BindedMaterial = NullAssetHandle;

	RendererData::ModelData::ModelData(glm::mat4 transformMatrix, glm::mat4 normalMatrix, int entityId) : TransformMatrix(transformMatrix), NormalMatrix(normalMatrix), EntityId(entityId) {}

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

		s_BindedShader = NullAssetHandle;
		s_BindedMaterial = NullAssetHandle;
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

		s_Data.AlphaMeshes.clear();
		s_Data.OpaqueMeshes.clear();
	}

	void Renderer::BeginScene(const Camera &camera, const glm::mat4 &transform, std::vector<Light> lights)
	{
		VXM_PROFILE_FUNCTION();
		s_Data.CameraBuffer.ViewProjectionMatrix = camera.GetProjectionMatrix() * glm::inverse(transform);
		glm::vec4 p = transform * glm::vec4{0,0,0,1};
		s_Data.CameraBuffer.CameraPosition = glm::vec4(glm::vec3(p) / p.w, 1);
		s_Data.CameraBuffer.CameraDirection = transform * glm::vec4{0,0,1,0};
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
		s_Data.LightBuffer.lightCount = std::min((int)lights.size(), MAX_LIGHT_COUNT);
		for (size_t i = 0; i < s_Data.LightBuffer.lightCount; ++i)
		{
			s_Data.LightBuffer.lights[i] = lights[i];
		}
		s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer, sizeof(RendererData::LightData));

		s_Data.AlphaMeshes.clear();
		s_Data.OpaqueMeshes.clear();
	}

	void Renderer::EndScene() {
		VXM_PROFILE_FUNCTION();

		for(const auto& mesh : s_Data.OpaqueMeshes)
		{
			Ref<Mesh> m = std::get<0>(mesh);
			s_Data.ModelBuffer.TransformMatrix = std::get<1>(mesh);
			s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(std::get<1>(mesh)));
			s_Data.ModelBuffer.EntityId = std::get<2>(mesh);

			s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

			MaterialField mat = m->GetMaterial();
			VXM_CORE_ASSERT(mat, "The material ID({}) is not valid.", mat.GetHandle());
			if(mat != s_BindedMaterial && mat) {
				s_BindedMaterial = mat;
				Ref<Material> matPtr = mat.GetAsset();
				s_Data.MaterialUniformBuffer->SetData(&matPtr->GetMaterialsParameters(), sizeof(MaterialParameters));

				ShaderField shader = matPtr->GetShaderHandle();
				VXM_CORE_ASSERT(shader, "The shader ID({}) from the material '{}' is not valid.", matPtr->GetMaterialName(), shader.GetHandle());
				matPtr->Bind(false);
				if (shader != s_BindedShader && shader) {
					shader.GetAsset()->Bind();
					s_BindedShader = shader;
				}
			}
			m->Bind();
			RenderCommand::DrawIndexed(std::get<0>(mesh)->GetVertexArray());
		}

		for(auto it = s_Data.AlphaMeshes.rbegin(); it != s_Data.AlphaMeshes.rend(); ++it)
		{
			auto& mesh = it->second;
			Ref<Mesh> m = std::get<0>(mesh);

			s_Data.ModelBuffer.TransformMatrix = std::get<1>(mesh);
			s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(std::get<1>(mesh)));
			s_Data.ModelBuffer.EntityId = std::get<2>(mesh);

			s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

			MaterialField mat = m->GetMaterial();
			VXM_CORE_ASSERT(mat, "The material ID({}) is not valid.", mat.GetHandle());
			if(mat != s_BindedMaterial && mat) {
				s_BindedMaterial = mat;
				Ref<Material> matPtr = mat.GetAsset();
				s_Data.MaterialUniformBuffer->SetData(&matPtr->GetMaterialsParameters(), sizeof(MaterialParameters));

				ShaderField shader = matPtr->GetShaderHandle();
				VXM_CORE_ASSERT(shader, "The shader ID({}) from the material '{}' is not valid.", matPtr->GetMaterialName(), shader.GetHandle());
				matPtr->Bind(false);
				if (shader != s_BindedShader && shader) {
					shader.GetAsset()->Bind();
					s_BindedShader = shader;
				}
			}

			RenderCommand::DrawIndexed(std::get<0>(mesh)->GetVertexArray());
		}

		RenderCommand::ClearBinding();
		s_BindedShader = NullAssetHandle;
		s_BindedMaterial = NullAssetHandle;
	}

	void Renderer::Submit(Ref<Shader>& shader, const Ref<VertexArray> &vertexArray, const glm::mat4& transform, int entityId) {
		VXM_PROFILE_FUNCTION();
		//        VXM_CORE_ASSERT(s_Data.CameraBuffer.ViewProjectionMatrix != glm::zero<glm::mat4>(), "A valid View Projection Matrix is required to submit data to the renderer.");


		s_Data.ModelBuffer.TransformMatrix = transform;
		s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ModelBuffer.EntityId = entityId;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		ShaderField shaderField = shader->Handle;
		VXM_CORE_ASSERT(shader, "The shader ID({}) is not valid.", shaderField.GetHandle());
		if (shaderField != s_BindedShader) {
			shader->Bind();
			s_BindedShader = shaderField;
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

		MaterialField mat = material;
		VXM_CORE_ASSERT(mat, "The material ID({}) is not valid.", mat.GetHandle());
		if(mat != s_BindedMaterial && mat) {
			s_BindedMaterial = mat;
			s_Data.MaterialUniformBuffer->SetData(&material->GetMaterialsParameters(), sizeof(MaterialParameters));

			ShaderField shader = material->GetShaderHandle();
			VXM_CORE_ASSERT(shader, "The shader ID({}) from the material '{}' is not valid.", material->GetMaterialName(), shader.GetHandle());
			material->Bind(false);
			if (shader != s_BindedShader && shader) {
				shader.GetAsset()->Bind();
				s_BindedShader = shader;
			}
		}
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Submit(const MeshGroup& meshGroup, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		// s_Data.ModelBuffer.TransformMatrix = transform;
		// s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(transform));
		// s_Data.ModelBuffer.EntityId = entityId;
		// s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		for (const auto& mesh : meshGroup)
		{
			if(mesh) {
				Submit(mesh.GetAsset(), transform, entityId);
			}
//			s_Data.MaterialUniformBuffer->SetData(&mesh->GetMaterial()->GetMaterialsParameters(), sizeof(MaterialParameters));
//			mesh->Bind();
//			auto& shaderName = mesh->GetMaterial()->GetShaderName();
//			if(shaderName != s_BindedShader)
//			{
//				mesh->GetMaterial()->Bind();
//				s_BindedShader = shaderName;
//			}
//			RenderCommand::DrawIndexed(mesh->GetVertexArray());
		}
	}

	void Renderer::Submit(Ref<Mesh> mesh, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		Vec4 center = transform*Vec4(mesh->GetBoundingBox().GetCenter(),1);
		center /= center.w;
		Real distance = Math::SqrMagnitude(Vec3(center) - Vec3(s_Data.CameraBuffer.CameraPosition));

		if(mesh->GetMaterial().GetAsset()->GetMaterialsParameters().AlphaMode == AlphaMode::Blend)
		{
			s_Data.AlphaMeshes.insert(std::make_pair(distance, std::tuple(mesh, transform, entityId)));
		}
		else
		{
			s_Data.OpaqueMeshes.emplace_back(std::tuple(mesh, transform, entityId));
		}
	}
/*
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
*/
	void Renderer::Submit(const Ref<Model>& model, const glm::mat4& transform, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		//model->Bind();
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