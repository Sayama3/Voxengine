//
// Created by ianpo on 26/05/2023.
//

#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/SystemHelper.hpp"
#include "Voxymore/Core.hpp"
#include "Voxymore/Logger.hpp"
#include <utility>

namespace Voxymore::Core {


    Shader Shader::CreateShaderFromSource(ShaderType shaderType, const std::string &shaderSource) {
        return Shader(shaderType, shaderSource);
    }

    Shader::Shader(const std::string &shaderPath, ShaderType shaderType) : m_ShaderType(shaderType),
                                                                           m_ShaderId(glCreateShader(m_ShaderType)) {
        std::string shaderSource = SystemHelper::ReadFile(shaderPath);
        const char *cstr = shaderSource.c_str();
        glShaderSource(m_ShaderId, 1, &cstr, nullptr);
        glCompileShader(m_ShaderId);
    }

    Shader::Shader(ShaderType shaderType, const std::string &shaderSource) : m_ShaderType(shaderType),
                                                                             m_ShaderId(glCreateShader(m_ShaderType)) {
        const char *cstr = shaderSource.c_str();
        glShaderSource(m_ShaderId, 1, &cstr, nullptr);
        glCompileShader(m_ShaderId);
    }

    Shader::~Shader() {
        glDeleteShader(m_ShaderId);
    }

    unsigned int Shader::GetId() const {
        return m_ShaderId;
    }

    bool Shader::CheckCompilation() const {
        int success;
        char infoLog[512];
        glGetShaderiv(m_ShaderId, GL_COMPILE_STATUS, &success);
        if (!success) {
            int maxLength = 0;
            glGetShaderiv(m_ShaderId, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<char> infoLog(maxLength);

            glGetShaderInfoLog(m_ShaderId, maxLength, &maxLength, &infoLog[0]);
            VXM_CORE_ERROR("Shader {0} Compilation failure!", ShaderTypeToString(this->m_ShaderType));
            VXM_CORE_ERROR("{0}", infoLog.data());
        }
        return success;
    }

    ShaderConstructor::ShaderConstructor(std::string path, ShaderType type) : shaderPath(std::move(path)),
                                                                              shaderType(type) {

    }

    Shader ShaderConstructor::CreateShader() const {
        return {shaderPath, shaderType};
    }
}