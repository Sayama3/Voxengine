//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Macros.hpp"
#include "Voxymore/OpenGL/OpenGLShader.hpp"
#include "Voxymore/Debug/Gizmos.hpp"

namespace Voxymore::Core
{
	enum class RenderingMode {
		None,
		Deferred,
		Forward,
	};
	static inline RendererData s_Data;
	static inline ShaderField s_BindedShader = NullAssetHandle;
	static inline MaterialField s_BindedMaterial = NullAssetHandle;
	static inline CubemapField s_Cubemap = NullAssetHandle;
	static inline ShaderField s_CubemapShader = NullAssetHandle;
	static inline ShaderField s_DeferredRenderShader = NullAssetHandle;
	static inline RenderingMode s_RenderingMode = RenderingMode::None;
	static inline glm::mat4 s_ViewMatrix;
	static inline glm::mat4 s_ProjMatrix;
	static inline Ref<Framebuffer> s_RenderFramebuffer = nullptr;
	static inline Ref<Framebuffer> s_DeferredFramebuffer = nullptr;

	static inline bool IsForwardRendering() {return s_RenderingMode == RenderingMode::Forward;}
	static inline bool IsDeferredRendering() {return s_RenderingMode == RenderingMode::Deferred;}

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

	void Renderer::SetupRenderer(ShaderField deferredRenderShader, Ref<Framebuffer> renderFramebuffer, Ref<Framebuffer> deferredFramebuffer) {
		s_DeferredRenderShader = deferredRenderShader;
		s_RenderFramebuffer = std::move(renderFramebuffer);
		s_DeferredFramebuffer = std::move(deferredFramebuffer);
		// RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
		//
		// if (s_DeferredFramebuffer) {
		// 	s_DeferredFramebuffer->Bind();
		// 	RenderCommand::Clear();
		// 	s_DeferredFramebuffer->ClearColorAttachment(4, -1); // Clear the ID texture
		// 	// s_RenderFramebuffer->ClearColorAttachment(1, -1); // Clear the ID texture
		// }
		//
		// if (s_RenderFramebuffer) {
		// 	s_RenderFramebuffer->Bind();
		// 	RenderCommand::Clear();
		// 	s_RenderFramebuffer->ClearColorAttachment(1, -1); // Clear the ID texture
		// }
	}


	void Renderer::BeginRendering(const Camera &camera, const glm::mat4 &transform, const std::vector<Light> &lights) {
		VXM_PROFILE_FUNCTION();

		s_Data.LightBuffer.lightCount = std::min(static_cast<int>(lights.size()), MAX_LIGHT_COUNT);
		for (size_t i = 0; i < s_Data.LightBuffer.lightCount; ++i)
		{
			s_Data.LightBuffer.lights[i] = lights[i];
		}

		const glm::vec4 p = transform * glm::vec4{0,0,0,1};
		s_ViewMatrix = glm::inverse(transform);
		s_ProjMatrix = camera.GetProjectionMatrix();
		s_Data.CameraBuffer.CameraPosition = glm::vec4(glm::vec3(p) / p.w, 1);
		s_Data.CameraBuffer.CameraDirection = transform * glm::vec4{0,0,1,0};
		s_Data.CameraBuffer.ViewProjectionMatrix = s_ProjMatrix * s_ViewMatrix;

		InitializeRendering();

	}

	void Renderer::BeginRendering(const EditorCamera &camera, const std::vector<Light> &lights) {
		VXM_PROFILE_FUNCTION();

		s_Data.LightBuffer.lightCount = std::min(static_cast<int>(lights.size()), MAX_LIGHT_COUNT);
		for (size_t i = 0; i < s_Data.LightBuffer.lightCount; ++i)
		{
			s_Data.LightBuffer.lights[i] = lights[i];
		}

		s_ViewMatrix = camera.GetViewMatrix();
		s_ProjMatrix = camera.GetProjectionMatrix();
		s_Data.CameraBuffer.CameraPosition = glm::vec4(camera.GetPosition(), 1);
		s_Data.CameraBuffer.CameraDirection = glm::vec4(camera.GetForwardDirection(), 0);
		s_Data.CameraBuffer.ViewProjectionMatrix = camera.GetViewProjection();

		InitializeRendering();
	}

	void Renderer::InitializeRendering() {
		VXM_PROFILE_FUNCTION();

		s_BindedShader = NullAssetHandle;
		s_BindedMaterial = NullAssetHandle;
		s_Cubemap = NullAssetHandle;
		s_CubemapShader = NullAssetHandle;
		s_RenderingMode = RenderingMode::None;

		s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer, sizeof(RendererData::LightData));
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));

		s_Data.AlphaMeshes.clear();
		s_Data.OpaqueMeshes.clear();

		if (s_DeferredFramebuffer) {
			s_DeferredFramebuffer->Bind();
			RenderCommand::SetClearColor({1,0,0,.5});
			RenderCommand::Clear();
			s_DeferredFramebuffer->ClearColorAttachment(4, -1); // Clear the ID texture
			s_DeferredFramebuffer->Unbind();
		}

		if (s_RenderFramebuffer) {
			s_RenderFramebuffer->Bind();
			RenderCommand::SetClearColor({0,1,0,0.5});
			RenderCommand::Clear();
			s_RenderFramebuffer->ClearColorAttachment(1, -1); // Clear the ID texture
			s_RenderFramebuffer->Unbind();
		}
	}

	void Renderer::BeginDeferredRendering() {
		if (!s_DeferredFramebuffer || !s_DeferredRenderShader) return;
		s_RenderingMode = RenderingMode::Deferred;
		s_DeferredFramebuffer->Bind();
		RenderCommand::EnableDepth();
	}

	void Renderer::EndDeferredRendering() {
		if (s_RenderingMode != RenderingMode::Deferred) return;
		s_RenderingMode = RenderingMode::None;
		s_DeferredFramebuffer->Unbind();
		s_RenderFramebuffer->Bind();
		VXM_CORE_ASSERT(s_DeferredRenderShader, "The Deferred Render Shader is invalid.");
		if (!s_DeferredRenderShader) return;

		s_DeferredRenderShader.GetAsset()->Bind();

		RenderCommand::SetupDeferredRender(
			s_DeferredFramebuffer->GetColorAttachmentRendererID(0),
			s_DeferredFramebuffer->GetColorAttachmentRendererID(1),
			s_DeferredFramebuffer->GetColorAttachmentRendererID(2),
			s_DeferredFramebuffer->GetColorAttachmentRendererID(3),
			s_DeferredFramebuffer->GetColorAttachmentRendererID(4),
			0
			);

		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		std::array<uint32_t, 6> indices {
			0,1,2,
			1,3,2,
		};

		auto vbo = VertexBuffer::Create(sizeof(quadVertices), &quadVertices);
		vbo->SetLayout(BufferLayout{{ShaderDataType::Float3, "Position"}, {ShaderDataType::Float2, "TexCoords"}});
		auto ibo = IndexBuffer::Create(indices.size(), indices.data());
		auto vao = VertexArray::Create();
		vao->AddVertexBuffer(vbo);
		vao->SetIndexBuffer(ibo);
		vao->Bind();
		RenderCommand::DrawIndexed(DrawMode::Triangles, vao);
		vao->Unbind();
		RenderCommand::CopyDepthAttachment(s_DeferredFramebuffer, s_RenderFramebuffer);
	}

	void Renderer::BeginForwardRendering() {
		s_RenderingMode = RenderingMode::Forward;
		s_RenderFramebuffer->Bind();
		RenderCommand::EnableDepth();
		for(const auto& mesh : s_Data.OpaqueMeshes)
		{
			DrawForwardMesh(std::get<0>(mesh), std::get<1>(mesh), std::get<2>(mesh));
		}
		s_Data.OpaqueMeshes.clear();
	}

	void Renderer::EndForwardRendering() {
/*
		RenderCommand::EnableWireframe();
		bool enableWireframe = true;
		for(auto it = Gizmos::get_cbegin_depth(); it != Gizmos::get_cend_depth(); ++it)
		{
			if(enableWireframe && !it->first.IsWiremesh) {
				RenderCommand::DisableWireframe();
				enableWireframe = false;
			}
			DrawForwardGizmo(it->second.Mesh, it->second.ModelMatrix);
		}
		if(enableWireframe) {RenderCommand::DisableWireframe(); enableWireframe = false;}
*/

		for(auto it = s_Data.AlphaMeshes.rbegin(); it != s_Data.AlphaMeshes.rend(); ++it)
		{
			auto& mesh = it->second;
			DrawForwardMesh(std::get<0>(mesh), std::get<1>(mesh), std::get<2>(mesh));
		}

/*
		RenderCommand::EnableWireframe();
		RenderCommand::DisableDepth();
		enableWireframe = true;
		for(auto it = Gizmos::get_cbegin_non_depth(); it != Gizmos::get_cend_non_depth(); ++it)
		{
			if(enableWireframe && !it->first.IsWiremesh) {
				RenderCommand::DisableWireframe();
				enableWireframe = false;
			}
			DrawForwardGizmo(it->second.Mesh, it->second.ModelMatrix);
		}
		if(enableWireframe) {RenderCommand::DisableWireframe(); enableWireframe = false;}
		RenderCommand::EnableDepth();
*/
		RenderCommand::ClearBinding();
		s_BindedShader = NullAssetHandle;
		s_BindedMaterial = NullAssetHandle;

		s_RenderingMode = RenderingMode::None;
	}

	void Renderer::EndRendering(CubemapField cubemap, ShaderField cubemapShader) {
		s_Cubemap = cubemap;
		s_CubemapShader = cubemapShader;
		if(s_Cubemap && s_CubemapShader) {
			DrawCubemap(s_ViewMatrix, s_ProjMatrix, s_Cubemap.GetAsset(), s_CubemapShader.GetAsset());
		}
	}

	void Renderer::BeginForwardScene(const EditorCamera &camera, std::vector<Light> lights, CubemapField cubemap, ShaderField cubemapShader)
	{
		VXM_PROFILE_FUNCTION();

		s_BindedShader = NullAssetHandle;
		s_BindedMaterial = NullAssetHandle;
		s_Cubemap = cubemap;
		s_CubemapShader = cubemapShader;
		s_RenderingMode = RenderingMode::Forward;

		s_Data.LightBuffer.lightCount = std::min((int)lights.size(), MAX_LIGHT_COUNT);
		for (size_t i = 0; i < s_Data.LightBuffer.lightCount; ++i)
		{
			s_Data.LightBuffer.lights[i] = lights[i];
		}
		s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer, sizeof(RendererData::LightData));

		s_Data.AlphaMeshes.clear();
		s_Data.OpaqueMeshes.clear();

		s_Data.CameraBuffer.CameraPosition = glm::vec4(camera.GetPosition(), 1);
		s_Data.CameraBuffer.CameraDirection = glm::vec4(camera.GetForwardDirection(), 0);
		if(s_Cubemap && s_CubemapShader) {
			DrawCubemap(camera.GetViewMatrix(), camera.GetProjectionMatrix(), s_Cubemap.GetAsset(), s_CubemapShader.GetAsset());
		}
		s_Data.CameraBuffer.ViewProjectionMatrix = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(RendererData::CameraData));
	}

	void Renderer::BeginForwardScene(const Camera &camera, const glm::mat4 &transform, std::vector<Light> lights, CubemapField cubemap, ShaderField cubemapShader)
	{
		VXM_PROFILE_FUNCTION();
		s_BindedShader = NullAssetHandle;
		s_BindedMaterial = NullAssetHandle;
		s_Cubemap = cubemap;
		s_CubemapShader = cubemapShader;
		s_RenderingMode = RenderingMode::Forward;

		s_Data.LightBuffer.lightCount = std::min((int)lights.size(), MAX_LIGHT_COUNT);
		for (size_t i = 0; i < s_Data.LightBuffer.lightCount; ++i)
		{
			s_Data.LightBuffer.lights[i] = lights[i];
		}
		s_Data.LightUniformBuffer->SetData(&s_Data.LightBuffer, sizeof(RendererData::LightData));

		s_Data.AlphaMeshes.clear();
		s_Data.OpaqueMeshes.clear();


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

	void Renderer::EndForwardScene() {
		VXM_PROFILE_FUNCTION();

		for(const auto& mesh : s_Data.OpaqueMeshes)
		{
			DrawForwardMesh(std::get<0>(mesh), std::get<1>(mesh), std::get<2>(mesh));
		}

		RenderCommand::EnableWireframe();
		bool enableWireframe = true;
		for(auto it = Gizmos::get_cbegin_depth(); it != Gizmos::get_cend_depth(); ++it)
		{
			if(enableWireframe && !it->first.IsWiremesh) {
				RenderCommand::DisableWireframe();
				enableWireframe = false;
			}
			DrawForwardGizmo(it->second.Mesh, it->second.ModelMatrix);
		}
		if(enableWireframe) {RenderCommand::DisableWireframe(); enableWireframe = false;}

		for(auto it = s_Data.AlphaMeshes.rbegin(); it != s_Data.AlphaMeshes.rend(); ++it)
		{
			auto& mesh = it->second;
			DrawForwardMesh(std::get<0>(mesh), std::get<1>(mesh), std::get<2>(mesh));
		}

		RenderCommand::EnableWireframe();
		RenderCommand::DisableDepth();
		enableWireframe = true;
		for(auto it = Gizmos::get_cbegin_non_depth(); it != Gizmos::get_cend_non_depth(); ++it)
		{
			if(enableWireframe && !it->first.IsWiremesh) {
				RenderCommand::DisableWireframe();
				enableWireframe = false;
			}
			DrawForwardGizmo(it->second.Mesh, it->second.ModelMatrix);
		}
		if(enableWireframe) {RenderCommand::DisableWireframe(); enableWireframe = false;}
		RenderCommand::EnableDepth();

		RenderCommand::ClearBinding();
		s_BindedShader = NullAssetHandle;
		s_BindedMaterial = NullAssetHandle;
		s_RenderingMode = RenderingMode::None;
	}

	void Renderer::DrawDeferredMesh(const Ref<Mesh>& m, const glm::mat4& modelMatrix, int entityId)
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
			s_Data.MaterialUniformBuffer->SetData(&matPtr->GetMaterialsParameters(), sizeof(MaterialParameters));
			matPtr->Bind(false);

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
	void Renderer::DrawForwardMesh(const Ref<Mesh>& m, const glm::mat4& modelMatrix, int entityId)
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
			s_Data.MaterialUniformBuffer->SetData(&matPtr->GetMaterialsParameters(), sizeof(MaterialParameters));
			matPtr->Bind(false);
			s_BindedMaterial = mat;

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

	void Renderer::DrawForwardGizmo(const Ref<Mesh>& m, const glm::mat4& modelMatrix)
	{
		VXM_PROFILE_FUNCTION();
		s_Data.ModelBuffer.TransformMatrix = modelMatrix;
		s_Data.ModelUniformBuffer->SetData(&s_Data.ModelBuffer, sizeof(RendererData::ModelData));

		MaterialField mat = m->GetMaterial();
		VXM_CORE_CHECK_ERROR(mat, "The material ID({}) is not valid.", mat.GetHandle().string());
		if(mat)
		{
			Ref<Material> matPtr = mat.GetAsset();
			s_Data.MaterialUniformBuffer->SetData(&matPtr->GetMaterialsParameters(), sizeof(MaterialParameters));

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

	void Renderer::SubmitOpaqueMesh(const Ref<Mesh>& mesh, const glm::mat4& modelMatrix, int entityId)
	{
		auto material = mesh->GetMaterial();
		if (!material) return;
		ShaderField shader = material.GetAsset()->GetShaderHandle();
		if (!shader) return;

		if (shader.GetAsset()->IsForward()) {
			if (IsForwardRendering()) {
				DrawForwardMesh(mesh, modelMatrix, entityId);
			} else {
				s_Data.OpaqueMeshes.emplace_back(std::tuple(mesh, modelMatrix, entityId));
			}
		}
		else if (IsDeferredRendering()) {
			DrawDeferredMesh(mesh, modelMatrix, entityId);
		} else {
			VXM_CORE_WARN("Trying to draw a non-forward rendering mesh with an incorrect render mode.");
		}

	}

	void Renderer::SubmitAlphaMesh(Real distance, const Ref<Mesh>& mesh, const glm::mat4& modelMatrix, int entityId)
	{
		s_Data.AlphaMeshes.insert(std::make_pair(distance, std::tuple(mesh, modelMatrix, entityId)));
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
			SubmitAlphaMesh(distance, mesh, transform, entityId);
		}
		else
		{
			SubmitOpaqueMesh(mesh, transform, entityId);
		}
	}

	void Renderer::GPUDraw(uint32_t count, uint32_t offset, DrawMode drawMode) {
		RenderCommand::GPUDraw(count, offset, drawMode);
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

} // Core