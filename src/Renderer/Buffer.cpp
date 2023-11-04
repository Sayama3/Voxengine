//
// Created by ianpo on 20/07/2023.
//
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/OpenGL/OpenGLBuffer.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Core/Logger.hpp"
namespace Voxymore::Core {

    Ref<VertexBuffer> Voxymore::Core::VertexBuffer::Create(uint32_t size, const void* vertices)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            {
                VXM_CORE_ASSERT(false, "{0} is not supported yet!", RendererAPIToString(RendererAPI::API::None));
                return nullptr;
            }
            case RendererAPI::API::OpenGL:
            {
                return CreateRef<OpenGLVertexBuffer>(size, vertices);
            }
        }

        VXM_CORE_ERROR("Renderer API '{0}' is not supported.", RendererAPIToString(Renderer::GetAPI()));

        return nullptr;
    }

    Ref<IndexBuffer> Voxymore::Core::IndexBuffer::Create(uint32_t size, const uint32_t* indices)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            {
                VXM_CORE_ASSERT(false, "{0} is not supported yet!", RendererAPIToString(RendererAPI::API::None))
                return nullptr;
            }
            case RendererAPI::API::OpenGL:
            {
                return CreateRef<OpenGLIndexBuffer>(size, indices);
            }
        }

        VXM_CORE_ERROR("Renderer API '{0}' is not supported.", RendererAPIToString(Renderer::GetAPI()));

        return nullptr;
    }
}