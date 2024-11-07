//
// Created by ianpo on 20/07/2023.
//
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/OpenGL/OpenGLBuffer.hpp"
#include "Voxymore/OpenGL/OpenGLSSBO.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Core/Logger.hpp"
#include "Voxymore/Core/Buffer.hpp"
#include "Voxymore/Renderer/Buffer.hpp"


namespace Voxymore::Core {

	Ref<VertexBuffer> Voxymore::Core::VertexBuffer::Create(uint32_t size, const void* vertices)
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
				return CreateRef<OpenGLVertexBuffer>(size, vertices);
			}
		}

		VXM_CORE_ERROR("Renderer API '{0}' is not supported.", RendererAPIToString(Renderer::GetAPI()));

		return nullptr;
	}

	Ref<IndexBuffer> Voxymore::Core::IndexBuffer::Create(uint32_t size, const uint32_t* indices)
	{
		VXM_PROFILE_FUNCTION();
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

	Ref<SSBO> SSBO::Create(uint64_t size, SSBO::Usage usage)
	{
		VXM_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				VXM_CORE_ASSERT(false, "{0} is not supported yet!", RendererAPIToString(RendererAPI::API::None))
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return CreateRef<OpenGLSSBO>(size, usage);
			}
		}

		VXM_CORE_ERROR("Renderer API '{0}' is not supported.", RendererAPIToString(Renderer::GetAPI()));
		return nullptr;
	}

	Ref<SSBO> SSBO::Create(Buffer data, SSBO::Usage usage)
	{
		VXM_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				VXM_CORE_ASSERT(false, "{0} is not supported yet!", RendererAPIToString(RendererAPI::API::None))
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return CreateRef<OpenGLSSBO>(data, usage);
			}
		}

		VXM_CORE_ERROR("Renderer API '{0}' is not supported.", RendererAPIToString(Renderer::GetAPI()));
		return nullptr;
	}
}