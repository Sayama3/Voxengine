//
// Created by ianpo on 21/08/2023.
//

#include "Voxymore/Renderer/Framebuffer.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/OpenGL/OpenGLFramebuffer.hpp"


namespace Voxymore::Core {

    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification &spec)
    {
		VXM_PROFILE_FUNCTION();
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            {
                VXM_CORE_ASSERT(false, "{0} is not supported yet!", RendererAPIToString(RendererAPI::API::None));
                return nullptr;
            }
            case RendererAPI::API::OpenGL:
            {
                return CreateRef<OpenGLFramebuffer>(spec);
            }
        }

        VXM_CORE_ERROR("Renderer API '{0}' is not supported.", RendererAPIToString(Renderer::GetAPI()));
        return nullptr;
    }
} // Voxymore
// Core