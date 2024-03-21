//
// Created by ianpo on 20/07/2023.
//

#include "OpenGLRenderAPI.hpp"
#include <glad/glad.h>
namespace Voxymore {
    namespace Core {
        void OpenGLRenderAPI::Init() {
            VXM_PROFILE_FUNCTION();
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
			glEnable(GL_CULL_FACE);
            glEnable(GL_DEPTH_TEST);
        }

        void OpenGLRenderAPI::Shutdown() {
            VXM_PROFILE_FUNCTION();
        }

        void OpenGLRenderAPI::SetClearColor(const glm::vec4 &color) {
            VXM_PROFILE_FUNCTION();
            glClearColor(color.r, color.g, color.b, color.a);
        }

        void OpenGLRenderAPI::Clear() {
            VXM_PROFILE_FUNCTION();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

		void OpenGLRenderAPI::ClearVABinding()
		{
			VXM_PROFILE_FUNCTION();
			glBindVertexArray(0);
		}

		void OpenGLRenderAPI::ClearShaderBinding()
		{
			VXM_PROFILE_FUNCTION();
			glUseProgram(0);
		}

        void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray> &vertexArray) {
            VXM_PROFILE_FUNCTION();
            glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        }

        void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            VXM_PROFILE_FUNCTION();
            glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
        }

		void OpenGLRenderAPI::UnbindTexture(uint32_t slot)
		{
			VXM_PROFILE_FUNCTION();
			glBindTextureUnit(slot, 0);
		}
	} // Voxymore
} // Core