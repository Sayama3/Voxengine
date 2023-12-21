//
// Created by ianpo on 29/08/2023.
//

#include "Voxymore/Renderer/UniformBuffer.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/OpenGL/OpenGLUniformBuffer.hpp"

namespace Voxymore
{
	namespace Core
	{
		Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
		{
			VXM_PROFILE_FUNCTION();
			switch (Renderer::GetAPI()) {

				case RendererAPI::API::None:
				{
					VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported yet.");
					break;
				}
				case RendererAPI::API::OpenGL:
				{
					return CreateRef<OpenGLUniformBuffer>(size, binding);
					break;
				}
			}
			return nullptr;
		}
	}// namespace Core
}// namespace Voxymore