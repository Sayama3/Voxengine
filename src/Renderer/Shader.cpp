//
// Created by ianpo on 20/07/2023.
//
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/OpenGL/OpenGLShader.hpp"
#include "Voxymore/SystemHelper.hpp"
#include "Voxymore/Logger.hpp"

namespace Voxymore::Core{

    Shader *Shader::CreateFromSource(const std::string &srcVertex, const std::string &srcFragment)
    {
        switch (Renderer::GetAPI()) {

            case RendererAPI::API::None:
                VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a shader.")
                return nullptr;
                break;
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(srcVertex, srcFragment);
                break;
        }
        VXM_CORE_ASSERT(false, "Render API '{0}' not supported.",RendererAPIToString(Renderer::GetAPI()))
        return nullptr;
    }

    Shader *Shader::CreateFromFile(const std::string &vertexFile, const std::string &fragmentFile) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                VXM_CORE_ASSERT(false, "RendererAPI::API::None is not supported to create a shader.")
                return nullptr;
                break;
            case RendererAPI::API::OpenGL:
            {
                std::string srcVertex = SystemHelper::ReadFile(vertexFile);
                std::string srcFragment = SystemHelper::ReadFile(fragmentFile);

                return new OpenGLShader(srcVertex, srcFragment);
            }
                break;
        }
        VXM_CORE_ASSERT(false, "Render API '{0}' not supported.",RendererAPIToString(Renderer::GetAPI()))
        return nullptr;
    }
}
