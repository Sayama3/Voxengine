//
// Created by ianpo on 20/07/2023.
//
#include "Voxymore/Renderer/Renderer.hpp"
#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/OpenGL/OpenGLShader.hpp"
#include "Voxymore/SystemHelper.hpp"
#include "Voxymore/Logger.hpp"

namespace Voxymore::Core{
    int GetShaderTypeValue(ShaderType shaderType) {
        switch (shaderType) {
            case COMPUTE_SHADER:
                break;
            case VERTEX_SHADER:
                break;
            case TESS_CONTROL_SHADER:
                break;
            case TESS_EVALUATION_SHADER:
                break;
            case GEOMETRY_SHADER:
                break;
            case FRAGMENT_SHADER:
                break;
        }

        VXM_CORE_ERROR("The shader type {0} is not handled.", ShaderTypeToString(shaderType));
        return -1;
    }

    ShaderParams CreateShaderParams(ShaderType type, std::string path) {
        return ShaderParams{
            SystemHelper::ReadFile(path),
            type,
        };
    }

    Shader *Shader::CreateShader(const ShaderParams& shader1) {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                break;
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(shader1);
        }
        return nullptr;
    }

    Shader *Shader::CreateShader(const ShaderParams& shader1, const ShaderParams& shader2) {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                break;
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(shader1, shader2);
        }
        return nullptr;
    }

    Shader *Shader::CreateShader(const ShaderParams& shader1, const ShaderParams& shader2, const ShaderParams& shader3) {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                break;
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(shader1, shader2, shader3);
        }
        return nullptr;
    }
}
