//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/VertexArray.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/OpenGL/OpenGLVertexArray.hpp"
#include "Voxymore/Core.hpp"

namespace Voxymore::Core {
    VertexArray *VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::None:
            {
                VXM_CORE_ERROR("{0} is not supported yet!", RendererAPIToString(RendererAPI::None));
                return nullptr;
            }

            case RendererAPI::OpenGL:
            {
                return new OpenGLVertexArray();
            }
        }

        VXM_CORE_ERROR("{0} is not supported yet!", RendererAPIToString(Renderer::GetAPI()));
        return nullptr;
    }
} // Core