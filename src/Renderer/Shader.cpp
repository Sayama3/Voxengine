//
// Created by ianpo on 20/07/2023.
//
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/OpenGL/OpenGLShader.hpp"
#include "Voxymore/Core/Logger.hpp"

namespace Voxymore::Core{

    Ref<Shader> Shader::Create(const std::string& name, const std::string &srcVertex, const std::string &srcFragment)
    {
        switch (Renderer::GetAPI()) {

            case RendererAPI::API::None:
                VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a shader.")
                return nullptr;
                break;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(name, srcVertex, srcFragment);
                break;
        }
        VXM_CORE_ASSERT(false, "Render API '{0}' not supported.",RendererAPIToString(Renderer::GetAPI()))
        return nullptr;
    }

    Ref<Shader> Shader::Create(const Path& path)
    {
        switch (Renderer::GetAPI()) {

            case RendererAPI::API::None:
                VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a shader.")
                return nullptr;
                break;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(path);
                break;
        }
        VXM_CORE_ASSERT(false, "Render API '{0}' not supported.", RendererAPIToString(Renderer::GetAPI()))
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& name, const Path& path)
    {
        switch (Renderer::GetAPI()) {

            case RendererAPI::API::None:
                VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a shader.")
                return nullptr;
                break;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(name, path);
                break;
        }
        VXM_CORE_ASSERT(false, "Render API '{0}' not supported.", RendererAPIToString(Renderer::GetAPI()))
        return nullptr;
    }

	ShaderLibrary* ShaderLibrary::s_Instance = nullptr;

    void ShaderLibrary::Add(const Ref<Shader> &shader) {
        const auto& name = shader->GetName();
#if VXM_DEBUG
        if(!m_Shaders.contains(name)) VXM_CORE_WARNING("Shader Library already contains '{0}'.", name);
#endif
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader> &shader) {
#if VXM_DEBUG
        if(!m_Shaders.contains(name)) VXM_CORE_WARNING("Shader Library already contains '{0}'.", name);
#endif
        m_Shaders[name] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const Path& path)
    {
        auto shader = Shader::Create(path);
        Add(shader);
        return m_Shaders[shader->GetName()];
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, const Path& path)
    {
        auto shader = Shader::Create(name, path);
        Add(name, shader);
        return m_Shaders[name];
    }

    Ref<Shader> ShaderLibrary::Get(const std::string &name) {
        VXM_CORE_ASSERT(m_Shaders.contains(name), "The shader library doesn't contain '{0}'.", name);
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string &name) const {
        return m_Shaders.contains(name);
    }

	ShaderLibrary& ShaderLibrary::GetInstance()
	{
		if(s_Instance == nullptr) s_Instance = new ShaderLibrary();
		return *s_Instance;
	}
}
