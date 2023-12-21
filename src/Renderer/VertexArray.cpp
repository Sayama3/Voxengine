//
// Created by ianpo on 20/07/2023.
//

#include "Voxymore/Renderer/VertexArray.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/OpenGL/OpenGLVertexArray.hpp"
#include "Voxymore/Core/Core.hpp"

namespace Voxymore::Core {
    Ref<VertexArray> VertexArray::Create() {
		VXM_PROFILE_FUNCTION();
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
            {
                VXM_CORE_ERROR("{0} is not supported yet!", RendererAPIToString(RendererAPI::API::None));
                return nullptr;
            }

            case RendererAPI::API::OpenGL:
            {
                return CreateRef<OpenGLVertexArray>();
            }
        }

        VXM_CORE_ERROR("{0} is not supported yet!", RendererAPIToString(Renderer::GetAPI()));
        return nullptr;
    }
} // Core