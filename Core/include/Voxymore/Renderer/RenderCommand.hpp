//
// Created by ianpo on 20/07/2023.
//

#pragma once

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
    private:
        static RendererAPI* s_RendererAPI;
    };

}