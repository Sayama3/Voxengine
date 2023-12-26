//
// Created by ianpo on 29/07/2023.
//

#include "Voxymore/Renderer/Texture.hpp"
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/OpenGL/OpenGLTexture2D.hpp"




namespace Voxymore::Core {
	Ref<Texture2D> Core::Texture2D::Create(const Path& path) {
		VXM_PROFILE_FUNCTION();
		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:
				VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a shader.")
				return nullptr;
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(path);
				break;
		}
		VXM_CORE_ASSERT(false, "Render API '{0}' not supported.",RendererAPIToString(Renderer::GetAPI()))
		return nullptr;
	}

	Ref<Texture2D> Core::Texture2D::Create(const std::filesystem::path& path) {
		VXM_PROFILE_FUNCTION();
		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:
				VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a shader.")
				return nullptr;
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(path);
				break;
		}
		VXM_CORE_ASSERT(false, "Render API '{0}' not supported.",RendererAPIToString(Renderer::GetAPI()))
		return nullptr;
	}

	Ref<Texture2D> Core::Texture2D::Create(const uint8_t* data, int width, int height, int channels) {
		VXM_PROFILE_FUNCTION();
		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:
				VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a shader.")
				return nullptr;
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(data, width, height, channels);
				break;
		}
		VXM_CORE_ASSERT(false, "Render API '{0}' not supported.",RendererAPIToString(Renderer::GetAPI()))
		return nullptr;
	}

	Ref<Texture2D> Core::Texture2D::Create(const uint16_t* data, int width, int height, int channels) {
		VXM_PROFILE_FUNCTION();
		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:
				VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a shader.")
				return nullptr;
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(data, width, height, channels);
				break;
		}
		VXM_CORE_ASSERT(false, "Render API '{0}' not supported.",RendererAPIToString(Renderer::GetAPI()))
		return nullptr;
	}

	Ref<Texture2D> Core::Texture2D::Create(const Texture2DSpecification& textureSpecs) {
		VXM_PROFILE_FUNCTION();
		switch (Renderer::GetAPI()) {

			case RendererAPI::API::None:
				VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a shader.")
				return nullptr;
				break;
			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLTexture2D>(textureSpecs);
				break;
		}
		VXM_CORE_ASSERT(false, "Render API '{0}' not supported.",RendererAPIToString(Renderer::GetAPI()))
		return nullptr;
	}
} // Voxymore
// Core