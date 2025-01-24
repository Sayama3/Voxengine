//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Mesh.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Renderer/Camera.hpp"
#include "Voxymore/Renderer/Cubemap.hpp"
#include "Voxymore/Renderer/EditorCamera.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/Renderer/PerspectiveCamera.hpp"
#include "Voxymore/Renderer/RenderCommand.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/UniformBuffer.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"
#include "Voxymore/Renderer/Model.hpp"
#include "Voxymore/Renderer/Light.hpp"
#include "Voxymore/Renderer/Framebuffer.hpp"
#include <map>


#define MAX_LIGHT_COUNT 20


namespace Voxymore::Core {

	struct RendererData
	{
		// TODO: Add Normal Matrix
		// TODO: Add MVP Matrix
		// Both Previous todo are to reduce the load on the GPU side.
		//  But it might need a bit of checking and more data to know if it's worth.

		struct CameraData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::vec4 CameraPosition;
			glm::vec4 CameraDirection;
		};

		struct ModelData
		{
			inline ModelData() = default;
			inline ~ModelData() = default;
			ModelData(glm::mat4 transformMatrix, glm::mat4 normalMatrix, int entityId);
			glm::mat4 TransformMatrix;
			glm::mat4 NormalMatrix;
			int EntityId;
		};

		struct LightData
		{
			Light lights[MAX_LIGHT_COUNT];
			int lightCount;
		};

		CameraData CameraBuffer;
		ModelData ModelBuffer;
		LightData LightBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
		Ref<UniformBuffer> ModelUniformBuffer;
		Ref<UniformBuffer> LightUniformBuffer;
		Ref<UniformBuffer> MaterialUniformBuffer;
		std::multimap<Real, std::tuple<const Ref<Mesh>, glm::mat4, int>> AlphaMeshes;
		std::vector<std::tuple<const Ref<Mesh>, glm::mat4, int>> OpaqueMeshes;
	};

	class Renderer {
	private:
		static void DrawCubemap(const glm::mat4& view, const glm::mat4& projection, const Ref<Cubemap>& cubemap, const Ref<Shader>& cubemapShader);
		static void Submit(const Ref<Model>& model, const Node& node, const glm::mat4& transform = glm::mat4(1.0f), int entityId = -1);
		static void DrawDeferredMesh(const Ref<Mesh>& mesh, const glm::mat4& modelMatrix, int entityId = -1);
		static void DrawForwardMesh(const Ref<Mesh>& mesh, const glm::mat4& modelMatrix, int entityId = -1);
		static void DrawForwardGizmo(const Ref<Mesh>& mesh, const glm::mat4& modelMatrix);

		static void SubmitOpaqueMesh(const Ref<Mesh>& mesh, const glm::mat4& modelMatrix, int entityId);
		static void SubmitAlphaMesh(Real distance, const Ref<Mesh>& mesh, const glm::mat4& modelMatrix, int entityId);
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void SetupRenderer(ShaderField deferredRenderShader, Ref<Framebuffer> renderFramebuffer, Ref<Framebuffer> deferredFramebuffer = nullptr);

		static void BeginRendering(const Camera& camera, const glm::mat4& transform, const std::vector<Light> &lights = {});
		static void BeginRendering(const EditorCamera& camera, const std::vector<Light> &lights = {});

		static void BeginDeferredRendering(); // Draw everything in the G Buffer and store the rest
		static void EndDeferredRendering(); // End the draw and sample the G-Buffer in the "back-buffer"

		static void BeginForwardRendering(); // Draw every opaque meshes and ignore the deferred materials
		static void EndForwardRendering(); // Stop drawing

		static void DrawCubemap(CubemapField cubemap = NullAssetHandle, ShaderField cubemapShader = NullAssetHandle); // finishing touch
		static void EndRendering(); // finishing touch


		[[deprecated]]
		static void BeginForwardScene(const Camera& camera, const glm::mat4& transform, std::vector<Light> lights = {}, CubemapField cubemap = NullAssetHandle, ShaderField cubemapShader = NullAssetHandle);
		[[deprecated]]
		static void BeginForwardScene(const EditorCamera& camera, std::vector<Light> lights = {}, CubemapField cubemap = NullAssetHandle, ShaderField cubemapShader = NullAssetHandle);
		[[deprecated]]
		static void EndForwardScene();


		static void Submit(const MeshGroup& mesh, const glm::mat4& transform = glm::mat4(1.0f), int entityId = -1);
		static void Submit(const Ref<Model>& model, const glm::mat4& transform = glm::mat4(1.0f), int entityId = -1);
		static void Submit(Ref<Mesh> model, const glm::mat4& transform = glm::mat4(1.0f), int entityId = -1);
//			static void Submit(const Mesh& model, const glm::mat4& transform = glm::mat4(1.0f), int entityId = -1);
		static void GPUDraw(uint32_t count, uint32_t offset = 0, DrawMode drawMode = DrawMode::Triangles);
	private:
		static void InitializeRendering();
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		//            static Scope<RendererData> s_Data;
	};

} // Voxymore
// Core
