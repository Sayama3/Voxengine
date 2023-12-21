//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Mesh.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Renderer/Camera.hpp"
#include "Voxymore/Renderer/EditorCamera.hpp"
#include "Voxymore/Renderer/Material.hpp"
#include "Voxymore/Renderer/PerspectiveCamera.hpp"
#include "Voxymore/Renderer/RenderCommand.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Renderer/UniformBuffer.hpp"
#include "Voxymore/Renderer/VertexArray.hpp"
#include "Voxymore/Renderer/Model.hpp"

namespace Voxymore {
	namespace Core {

		struct RendererData
		{
			// TODO: Add Normal Matrix
			// TODO: Add MVP Matrix
			// Both Previous todo are to reduce the load on the GPU side.
			//  But it might need a bit of checking and more data to know if it's worth.

			struct CameraData
			{
				glm::mat4 ViewProjectionMatrix;
			};
			struct ModelData
			{
				glm::mat4 TransformMatrix;
				int EntityId;
			};
			CameraData CameraBuffer;
			ModelData ModelBuffer;
			Ref<UniformBuffer> CameraUniformBuffer;
			Ref<UniformBuffer> ModelUniformBuffer;
			Ref<UniformBuffer> LightUniformBuffer;
			Ref<UniformBuffer> MaterialUniformBuffer;
		};

		class Renderer {
		private:
			static void Submit(const Ref<Model>& model, const Node& node, const glm::mat4& transform = glm::mat4(1.0f), int entityId = -1);
		public:
			static void Init();
			static void Shutdown();
			static void OnWindowResize(uint32_t width, uint32_t height);

			static void BeginScene(const Camera& camera, const glm::mat4& transform);
			static void BeginScene(const EditorCamera& camera);
			static void BeginScene(const PerspectiveCamera& camera);
			static void EndScene();

			static void Submit(Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f), int entityId = -1);
			static void Submit(Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f), int entityId = -1);
			static void Submit(const MeshGroup& mesh, const glm::mat4& transform = glm::mat4(1.0f), int entityId = -1);
			static void Submit(const Ref<Model>& model, const glm::mat4& transform = glm::mat4(1.0f), int entityId = -1);

			inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		private:
			//            static Scope<RendererData> s_Data;
		};

	} // Voxymore
} // Core
