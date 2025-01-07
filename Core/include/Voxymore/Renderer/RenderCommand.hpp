//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Framebuffer.hpp"
#include "RendererAPI.hpp"


namespace Voxymore::Core {
    class RenderCommand{
    public:
        inline static void Init(){
        	VXM_PROFILE_FUNCTION();
            s_RendererAPI->Init();
        }
        inline static void Shutdown(){
        	VXM_PROFILE_FUNCTION();
            s_RendererAPI->Shutdown();
        }
        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
        	VXM_PROFILE_FUNCTION();
            s_RendererAPI->SetViewport(x, y, width, height);
        }
        inline static void DrawIndexed(const Ref<VertexArray> &vertexArray) {
        	VXM_PROFILE_FUNCTION();
            s_RendererAPI->DrawIndexed(vertexArray);
        }
        inline static void DrawIndexed(DrawMode drawMode, const Ref<VertexArray> &vertexArray) {
        	VXM_PROFILE_FUNCTION();
            s_RendererAPI->DrawIndexed(drawMode, vertexArray);
        }
        inline static void DrawPatches(uint32_t patchCount) {
        	VXM_PROFILE_FUNCTION();
            s_RendererAPI->DrawPatches(patchCount);
        }

		inline static void GPUDraw(uint32_t count, uint32_t offset, DrawMode drawMode) {
	        VXM_PROFILE_FUNCTION();
        	s_RendererAPI->GPUDraw(count, offset, drawMode);
        }


        inline static void SetClearColor(const glm::vec4& color) {
        	VXM_PROFILE_FUNCTION();
            s_RendererAPI->SetClearColor(color);
        }
        inline static void Clear() {
        	VXM_PROFILE_FUNCTION();
            s_RendererAPI->Clear();
        }
		inline static void ClearBinding()
		{
			VXM_PROFILE_FUNCTION();
			s_RendererAPI->ClearVABinding();
			s_RendererAPI->ClearShaderBinding();
		}

		inline static void UnbindTexture(uint32_t slot)
		{
			VXM_PROFILE_FUNCTION();
			s_RendererAPI->UnbindTexture(slot);
		}

		inline static void DrawCubemap(Ref<Cubemap> cubemap, Ref<Shader> shader, Ref<VertexArray> mesh) {
			VXM_PROFILE_FUNCTION();
			VXM_CORE_ASSERT(cubemap != nullptr, "Failed to draw the cubemap. The provided cubemap is not initialized.")
			VXM_CORE_ASSERT(shader != nullptr, "Failed to draw the cubemap. The provided shader is not initialized.")
			VXM_CORE_ASSERT(mesh != nullptr, "Failed to draw the cubemap. The provided mesh is not initialized.")
			if(!cubemap || !shader || !mesh) return;
			s_RendererAPI->DrawCubemap(cubemap, shader, mesh);
		}
		inline static void DisableDepth() {VXM_PROFILE_FUNCTION(); s_RendererAPI->DisableDepth();}
		inline static void EnableDepth() {VXM_PROFILE_FUNCTION(); s_RendererAPI->EnableDepth();}

		inline static void EnableWireframe() {VXM_PROFILE_FUNCTION(); s_RendererAPI->EnableWireframe();}
		inline static void DisableWireframe() {VXM_PROFILE_FUNCTION(); s_RendererAPI->DisableWireframe();}

		inline static void EnableDoubleSided() {VXM_PROFILE_FUNCTION(); s_RendererAPI->EnableDoubleSided();}
		inline static void DisableDoubleSided() {VXM_PROFILE_FUNCTION(); s_RendererAPI->DisableDoubleSided();}
		inline static void CopyColorAttachment(Ref<Framebuffer> source, uint32_t sourceColorAttachmentIndex, Ref<Framebuffer> target, uint32_t targetColorAttachmentIndex) {VXM_PROFILE_FUNCTION(); s_RendererAPI->CopyColorBuffer(std::move(source), sourceColorAttachmentIndex, std::move(target), targetColorAttachmentIndex);}
		inline static void CopyDepthAttachment(Ref<Framebuffer> source, Ref<Framebuffer> target) {VXM_PROFILE_FUNCTION(); s_RendererAPI->CopyDepthBuffer(std::move(source), std::move(target));}

        inline static void SetupDeferredRender(uint32_t PositionTexID, uint32_t NormalTexID, uint32_t TexCoordTexID, uint32_t ColorTexID, uint32_t EntityIdTexID, uint32_t DepthTexID) {
        	VXM_PROFILE_FUNCTION();
	        s_RendererAPI->SetupDeferredRender(PositionTexID, NormalTexID, TexCoordTexID, ColorTexID, EntityIdTexID, DepthTexID);
        }

    private:
        static RendererAPI* s_RendererAPI;
    };

}