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
	static CubemapField s_Cubemap = NullAssetHandle;
	static ShaderField s_CubemapShader = NullAssetHandle;

	static Ref<Mesh> s_CubemapMesh;

	RendererData::ModelData::ModelData(glm::mat4 transformMatrix, glm::mat4 normalMatrix, int entityId) : TransformMatrix(transformMatrix), NormalMatrix(normalMatrix), EntityId(entityId) {}

	void Renderer::DrawCubemap(const glm::mat4& view, const glm::mat4& projection, const Ref<Cubemap>& cubemap, const Ref<Shader>& cubemapShader)
	{
		VXM_PROFILE_FUNCTION();
		s_Data.CameraBuffer.ViewProjectionMatrix = projection * glm::mat4(glm::mat3(view));
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
		RenderCommand::DrawCubemap(cubemap, cubemapShader, s_CubemapMesh->GetVertexArray());
	}

	void Renderer::Init() {
		VXM_PROFILE_FUNCTION();
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(RendererData::CameraData), 0);
		s_Data.ModelUniformBuffer = UniformBuffer::Create(sizeof(RendererData::ModelData), 1);
		s_Data.LightUniformBuffer = UniformBuffer::Create(sizeof(RendererData::LightData), 2);
		s_Data.MaterialUniformBuffer = UniformBuffer::Create(sizeof(MaterialParameters), 3);

		RenderCommand::Init();

		s_CubemapMesh = CreateRef<Mesh>(std::vector<Vertex>{
										 Vertex({-1.0f,  1.0f, -1.0f}),
										 Vertex({-1.0f, -1.0f, -1.0f}),
										 Vertex({1.0f, -1.0f, -1.0f}),
										 Vertex({1.0f, -1.0f, -1.0f}),
										 Vertex({1.0f,  1.0f, -1.0f}),
										 Vertex({-1.0f,  1.0f, -1.0f}),

										 Vertex({-1.0f, -1.0f,  1.0f}),
										 Vertex({-1.0f, -1.0f, -1.0f}),
										 Vertex({-1.0f,  1.0f, -1.0f}),
										 Vertex({-1.0f,  1.0f, -1.0f}),
										 Vertex({-1.0f,  1.0f,  1.0f}),
										 Vertex({-1.0f, -1.0f,  1.0f}),

										 Vertex({1.0f, -1.0f, -1.0f}),
										 Vertex({1.0f, -1.0f,  1.0f}),
										 Vertex({1.0f,  1.0f,  1.0f}),
										 Vertex({1.0f,  1.0f,  1.0f}),
										 Vertex({1.0f,  1.0f, -1.0f}),
										 Vertex({1.0f, -1.0f, -1.0f}),

										 Vertex({-1.0f, -1.0f,  1.0f}),
										 Vertex({-1.0f,  1.0f,  1.0f}),
										 Vertex({1.0f,  1.0f,  1.0f}),
										 Vertex({1.0f,  1.0f,  1.0f}),
										 Vertex({1.0f, -1.0f,  1.0f}),
										 Vertex({-1.0f, -1.0f,  1.0f}),

										 Vertex({-1.0f,  1.0f, -1.0f}),
										 Vertex({1.0f,  1.0f, -1.0f}),
										 Vertex({1.0f,  1.0f,  1.0f}),
										 Vertex({1.0f,  1.0f,  1.0f}),
										 Vertex({-1.0f,  1.0f,  1.0f}),
										 Vertex({-1.0f,  1.0f, -1.0f}),

										 Vertex({-1.0f, -1.0f, -1.0f}),
										 Vertex({-1.0f, -1.0f,  1.0f}),
										 Vertex({1.0f, -1.0f, -1.0f}),
										 Vertex({1.0f, -1.0f, -1.0f}),
										 Vertex({-1.0f, -1.0f,  1.0f}),
										 Vertex({1.0f, -1.0f,  1.0f})
		});
	}

	void Renderer::Shutdown() {
		VXM_PROFILE_FUNCTION();

		s_CubemapMesh = nullptr;
		RenderCommand::Shutdown();
	}

	void Renderer::BeginScene(const EditorCamera &camera, std::vector<Light> lights, CubemapField cubemap, ShaderField cubemapShader)
	{
		VXM_PROFILE_FUNCTION();

		s_BindedShader = NullAssetHandle;
		s_BindedMaterial = NullAssetHandle;
		s_Cubemap = cubemap;
		s_CubemapShader = cubemapShader;

		s_Data.LightBuffer.lightCount = std::min((int)lights.size(), MAX_LIGHT_COUNT);
		for (size_t i = 0; i < s_Data.LightBuffer.lightCount; ++i)
		{
			s_Data.LightBuffer.lights[i] = lights[i];
		}
		s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer, sizeof(RendererData::LightData));

		s_Data.AlphaMeshes.clear();
		s_Data.OpaqueMeshes.clear();
		s_Data.DepthGizmos.clear();
		s_Data.NonDepthGizmos.clear();

		s_Data.CameraBuffer.CameraPosition = glm::vec4(camera.GetPosition(), 1);
		s_Data.CameraBuffer.CameraDirection = glm::vec4(camera.GetForwardDirection(), 0);
		if(s_Cubemap && s_CubemapShader) {
			DrawCubemap(camera.GetViewMatrix(), camera.GetProjectionMatrix(), s_Cubemap.GetAsset(), s_CubemapShader.GetAsset());
		}
		s_Data.CameraBuffer.ViewProjectionMatrix = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
	}

	void Renderer::BeginScene(const Camera &camera, const glm::mat4 &transform, std::vector<Light> lights, CubemapField cubemap, ShaderField cubemapShader)
	{
		VXM_PROFILE_FUNCTION();
		s_BindedShader = NullAssetHandle;
		s_BindedMaterial = NullAssetHandle;
		s_Cubemap = cubemap;
		s_CubemapShader = cubemapShader;

		s_Data.LightBuffer.lightCount = std::min((int)lights.size(), MAX_LIGHT_COUNT);
		for (size_t i = 0; i < s_Data.LightBuffer.lightCount; ++i)
		{
			s_Data.LightBuffer.lights[i] = lights[i];
		}
		s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer, sizeof(RendererData::LightData));

		s_Data.AlphaMeshes.clear();
		s_Data.OpaqueMeshes.clear();
		s_Data.DepthGizmos.clear();
		s_Data.NonDepthGizmos.clear();


		const glm::vec4 p = transform * glm::vec4{0,0,0,1};
		const auto view = glm::inverse(transform);
		const auto proj = camera.GetProjectionMatrix();
		s_Data.CameraBuffer.CameraPosition = glm::vec4(glm::vec3(p) / p.w, 1);
		s_Data.CameraBuffer.CameraDirection = transform * glm::vec4{0,0,1,0};
		if(s_Cubemap && s_CubemapShader) {
			DrawCubemap(view, proj, s_Cubemap.GetAsset(), s_CubemapShader.GetAsset());
		}
		s_Data.CameraBuffer.ViewProjectionMatrix = proj * view;
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
	}

	void Renderer::DrawMesh(const Ref<Mesh>& m, const glm::mat4& modelMatrix, int entityId)
	{
		VXM_PROFILE_FUNCTION();
		s_Data.ModelBuffer.TransformMatrix = modelMatrix;
		s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(modelMatrix));
		s_Data.ModelBuffer.EntityId = entityId;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		MaterialField mat = m->GetMaterial();
		VXM_CORE_CHECK_ERROR(mat, "The material ID({}) is not valid.", mat.GetHandle().string());
		if(mat)
		{
			Ref<Material> matPtr = mat.GetAsset();
			//if(mat != s_BindedMaterial && mat) {
			s_Data.MaterialUniformBuffer->SetData(&matPtr->GetMaterialsParameters(), sizeof(MaterialParameters));
			matPtr->Bind(false);
			s_BindedMaterial = mat;
			//}

			ShaderField shader = matPtr->GetShaderHandle();
			VXM_CORE_CHECK_ERROR(shader, "The shader ID({}) from the material '{}' is not valid.", matPtr->GetMaterialName(), shader.GetHandle().string());
			if (shader != s_BindedShader && shader) {
				shader.GetAsset()->Bind();
				s_BindedShader = shader;
			}
		}
		m->Bind();
		RenderCommand::DrawIndexed(m->GetDrawMode(), m->GetVertexArray());
	}

	void Renderer::DrawGizmo(const Ref<Mesh>& m, const glm::mat4& modelMatrix)
	{
		VXM_PROFILE_FUNCTION();
		s_Data.ModelBuffer.TransformMatrix = modelMatrix;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		MaterialField mat = m->GetMaterial();
		VXM_CORE_CHECK_ERROR(mat, "The material ID({}) is not valid.", mat.GetHandle().string());
		if(mat)
		{
			Ref<Material> matPtr = mat.GetAsset();

			ShaderField shader = matPtr->GetShaderHandle();
			VXM_CORE_CHECK_ERROR(shader, "The shader ID({}) from the material '{}' is not valid.", matPtr->GetMaterialName(), shader.GetHandle().string());
			if (shader != s_BindedShader && shader) {
				shader.GetAsset()->Bind();
				s_BindedShader = shader;
			}
		}
		m->Bind();
		RenderCommand::DrawIndexed(m->GetDrawMode(), m->GetVertexArray());
	}

	void Renderer::EndScene() {
		VXM_PROFILE_FUNCTION();

		for(const auto& mesh : s_Data.OpaqueMeshes)
		{
			DrawMesh(std::get<0>(mesh), std::get<1>(mesh), std::get<2>(mesh));
		}

		for(const auto& [mesh, matrix, wireframe] : s_Data.DepthGizmos)
		{
			if(wireframe) RenderCommand::EnableWireframe();
			DrawGizmo(mesh, matrix);
			if(wireframe) RenderCommand::DisableWireframe();
		}

		for(auto it = s_Data.AlphaMeshes.rbegin(); it != s_Data.AlphaMeshes.rend(); ++it)
		{
			auto& mesh = it->second;
			DrawMesh(std::get<0>(mesh), std::get<1>(mesh), std::get<2>(mesh));
		}

		RenderCommand::DisableDepth();
		for(const auto& [mesh, matrix, wireframe] : s_Data.NonDepthGizmos)
		{
			if(wireframe) RenderCommand::EnableWireframe();
			DrawGizmo(mesh, matrix);
			if(wireframe) RenderCommand::DisableWireframe();
		}
		RenderCommand::EnableDepth();

//		VXM_CORE_INFO(R"(Mesh Drawn :
//	- OpaqueMeshes : {}
//	- DepthGizmos : {}
//	- AlphaMeshes : {}
//	- NonDepthGizmos : {}
//	- Total : {})", s_Data.OpaqueMeshes.size(), s_Data.DepthGizmos.size(), s_Data.AlphaMeshes.size(), s_Data.NonDepthGizmos.size(),(s_Data.OpaqueMeshes.size() + s_Data.DepthGizmos.size() + s_Data.AlphaMeshes.size() + s_Data.NonDepthGizmos.size()));
		RenderCommand::ClearBinding();
		s_BindedShader = NullAssetHandle;
		s_BindedMaterial = NullAssetHandle;

		//TODO: Find something better.
		{

		}
	}

	void Renderer::Submit(Ref<Shader>& shader, const Ref<VertexArray> &vertexArray, const glm::mat4& transform, int entityId) {
		VXM_PROFILE_FUNCTION();
		//        VXM_CORE_ASSERT(s_Data.CameraBuffer.ViewProjectionMatrix != glm::zero<glm::mat4>(), "A valid View Projection Matrix is required to submit data to the renderer.");


		s_Data.ModelBuffer.TransformMatrix = transform;
		s_Data.ModelBuffer.NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ModelBuffer.EntityId = entityId;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		ShaderField shaderField = shader->Handle;
		VXM_CORE_ASSERT(shader, "The shader ID({}) is not valid.", shaderField.GetHandle().string());
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

		MaterialField mat = material->Handle;
		VXM_CORE_ASSERT(mat, "The material ID({}) is not valid.", mat.GetHandle().string());
		Ref<Material> matPtr = mat.GetAsset();
		//if(mat != s_BindedMaterial && mat) {
			s_Data.MaterialUniformBuffer->SetData(&matPtr->GetMaterialsParameters(), sizeof(MaterialParameters));
			matPtr->Bind(false);
			s_BindedMaterial = mat;
		//}

		ShaderField shader = matPtr->GetShaderHandle();
		VXM_CORE_ASSERT(shader, "The shader ID({}) from the material '{}' is not valid.", matPtr->GetMaterialName(), shader.GetHandle().string());
		if (shader != s_BindedShader && shader) {
			shader.GetAsset()->Bind();
			s_BindedShader = shader;
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
		VXM_CORE_CHECK(model, "No model sent to be drawn.");
		if(!model) return;
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

	void Renderer::SubmitGizmo(Ref<Mesh> model, const glm::mat4& matrix, bool ignoreDepth, bool wireModel)
	{
		if(ignoreDepth) {
			s_Data.NonDepthGizmos.emplace_back(std::move(model), matrix, wireModel);
		} else {
			s_Data.DepthGizmos.emplace_back(std::move(model), matrix, wireModel);
		}
	}

} // Core