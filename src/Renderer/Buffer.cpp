//
// Created by ianpo on 20/07/2023.
//
#include "Voxymore/Core.hpp"
#include "Voxymore/OpenGL/OpenGLBuffer.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Logger.hpp"
namespace Voxymore::Core {

    VertexBuffer* Voxymore::Core::VertexBuffer::Create(uint32_t size, void* vertices)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::None:
            {
                VXM_CORE_ERROR("{0} is not supported yet!", RendererAPIToString(RendererAPI::None));
                return nullptr;
            }
            case RendererAPI::OpenGL:
            {
                return new OpenGLVertexBuffer(size, vertices);
            }
        }

        VXM_CORE_ERROR("Renderer API '{0}' is not supported.", RendererAPIToString(Renderer::GetAPI()));

        return nullptr;
    }

    IndexBuffer* Voxymore::Core::IndexBuffer::Create(uint32_t size, uint32_t *indices)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::None:
            {
                VXM_CORE_ERROR("{0} is not supported yet!", RendererAPIToString(RendererAPI::None));
                return nullptr;
            }
            case RendererAPI::OpenGL:
            {
                return new OpenGLIndexBuffer(size, indices);
            }
        }

        VXM_CORE_ERROR("Renderer API '{0}' is not supported.", RendererAPIToString(Renderer::GetAPI()));

        return nullptr;
    }
}