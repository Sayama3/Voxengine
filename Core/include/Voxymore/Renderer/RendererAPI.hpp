//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "VertexArray.hpp"
#include "Cubemap.hpp"
#include "Framebuffer.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Renderer/Shader.hpp"

namespace Voxymore::Core {

	enum class DrawMode {
		Points,
		Lines,
		Triangles,
	};

	class RendererAPI {
	public:

		enum class API {
			None = 0,
			OpenGL = 1,
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void ClearVABinding() = 0;
		virtual void ClearShaderBinding() = 0;
		virtual void UnbindTexture(uint32_t slot) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawIndexed(DrawMode drawMode, const Ref<VertexArray>& vertexArray) = 0;

		virtual void DrawPatches(uint32_t verticesPerPatch) = 0;
		virtual void GPUDraw(uint32_t count, uint32_t offset, DrawMode drawMode) = 0;

		virtual void DrawCubemap(Ref<Cubemap> cubemap, Ref<Shader> shader, Ref<VertexArray> mesh) = 0;

		virtual void DisableDepth() = 0;
		virtual void EnableDepth() = 0;

		virtual void EnableWireframe() = 0;
		virtual void DisableWireframe() = 0;

		virtual void EnableDoubleSided() = 0;
		virtual void DisableDoubleSided() = 0;
		virtual void CopyColorBuffer(Ref<Framebuffer> source, uint32_t sourceColorAttachmentIndex, Ref<Framebuffer> target, uint32_t targetColorAttachmentIndex) = 0;
		virtual void CopyDepthBuffer(Ref<Framebuffer> source, Ref<Framebuffer> target) = 0;

		virtual void SetupDeferredRender(uint32_t positionTexID, uint32_t normalTexID, uint32_t texCoordTexID, uint32_t colorTexID, uint32_t entityIdTexID, uint32_t depthTexID) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

	static inline std::string RendererAPIToString(RendererAPI::API api){
		VXM_PROFILE_FUNCTION();
		switch (api) {
			case RendererAPI::API::None:
				return "RendererAPI::None";
			case RendererAPI::API::OpenGL:
				return "RendererAPI::OpenGL";
		}
		return "Unknown";
	}

	static inline std::ostream& operator<<(std::ostream& os, const RendererAPI::API& api)
	{
		return os << RendererAPIToString(api);
	}


}