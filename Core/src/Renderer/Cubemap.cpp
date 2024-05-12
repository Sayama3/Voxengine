//
// Created by Iannis on 10/05/2024.
//

#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Renderer/RendererAPI.hpp"
#include "Voxymore/Renderer/Cubemap.hpp"
#include "Voxymore/OpenGL/OpenGLCubemap.hpp"


namespace Voxymore::Core
{
	Ref<Cubemap> Cubemap::Create(const CubemapSpecification &textureSpecs)
	{
		VXM_PROFILE_FUNCTION();
		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:
				VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a cube map.")
				return nullptr;
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLCubemap>(textureSpecs);
				break;
		}
		VXM_CORE_ASSERT(false, "Render API '{0}' not supported.", RendererAPIToString(Renderer::GetAPI()))
		return nullptr;
	}
} // namespace Voxymore::Core
