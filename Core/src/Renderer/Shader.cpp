//
// Created by ianpo on 20/07/2023.
//
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/OpenGL/OpenGLShader.hpp"
#include "Voxymore/Core/Logger.hpp"

namespace Voxymore::Core{

	Ref<Shader> Shader::Create(const std::string& name, const std::vector<ShaderSourceField>& sources)
	{
		VXM_PROFILE_FUNCTION();
		std::unordered_map<ShaderType, ShaderSourceField> map;
		for (auto& src : sources) {
			if(src) {
				map.insert_or_assign(src.GetAsset()->Type, src);
			}
		}
		return Create(name, map);
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::unordered_map<ShaderType, ShaderSourceField>& sources)
	{
		VXM_PROFILE_FUNCTION();
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a shader.")
				return nullptr;
				break;
			case RendererAPI::API::OpenGL: {
				return CreateRef<OpenGLShader>(name, sources);
			} break;
		}
		VXM_CORE_ASSERT(false, "Render API '{0}' not supported.", RendererAPIToString(Renderer::GetAPI()))
		return nullptr;
	}

	ShaderSource::ShaderSource(ShaderType type) : Type(type)
	{
	}

	ShaderSource::ShaderSource(std::string source) : Source(source)
	{
	}

	ShaderSource::ShaderSource(ShaderType type, std::string source) : Type(type), Source(source)
	{
	}
}
