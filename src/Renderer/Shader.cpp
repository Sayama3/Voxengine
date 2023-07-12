//
// Created by ianpo on 26/05/2023.
//

#include "Renderer/Shader.hpp"
#include "SystemHelper.hpp"
#include <iostream>
#include <utility>

namespace Voxymore::Core::Renderer {
    Shader::Shader(const std::string &shaderPath, ShaderType shaderType) : m_ShaderType(shaderType),
                                                                           m_ShaderId(glCreateShader(m_ShaderType)) {
        std::string vertexShaderSource = SystemHelper::ReadFile(shaderPath);
        const char *cstr = vertexShaderSource.c_str();
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
            glGetShaderInfoLog(m_ShaderId, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::" << ShaderTypeToString(m_ShaderType) << "::COMPILATION_FAILED\n" << infoLog
                      << std::endl;
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