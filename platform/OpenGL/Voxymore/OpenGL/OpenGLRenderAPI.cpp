//
// Created by ianpo on 20/07/2023.
//

#include "OpenGLRenderAPI.hpp"
#include <glad/glad.h>
namespace Voxymore {
    namespace Core {
        void OpenGLRenderAPI::Init() {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_CULL_FACE);
            glEnable(GL_DEPTH_TEST);
        }

        void OpenGLRenderAPI::Shutdown() {
        }

        void OpenGLRenderAPI::SetClearColor(const glm::vec4 &color) {
            glClearColor(color.r, color.g, color.b, color.a);
        }

        void OpenGLRenderAPI::Clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray> &vertexArray) {
            glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        }

        void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
        }
    } // Voxymore
} // Core