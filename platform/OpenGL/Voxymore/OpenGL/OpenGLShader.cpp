//
// Created by ianpo on 20/07/2023.
//

#include "OpenGLShader.hpp"
#include "Voxymore/Core/Core.hpp"
#include "Voxymore/Core/Logger.hpp"
#include <glad/glad.h>
#include <filesystem>
#include <regex>
#include <string>

#ifndef NEWLINE
#define NEWLINE "\n"
#endif

#define VERTEX_TYPE "__TYPE_VERTEX_SHADER__"
#define FRAGMENT_TYPE "__TYPE_FRAGMENT_SHADER__"
#define PIXEL_TYPE "__TYPE_PIXEL_SHADER__"
#define GEOMETRY_TYPE "__TYPE_GEOMETRY_SHADER__"
#define COMPUTE_TYPE "__TYPE_COMPUTE_SHADER__"
#define TESS_CONTROL_SHADER_TYPE "__TYPE_TESS_CONTROL_SHADER__"
#define TESS_EVALUATION_SHADER_TYPE "__TYPE_TESS_EVALUATION_SHADER__"


namespace Voxymore::Core {
    unsigned int GetShaderTypeID(ShaderType shaderType) {
        switch (shaderType) {
            case ShaderType::COMPUTE_SHADER:
                return GL_COMPUTE_SHADER;
                break;
            case ShaderType::VERTEX_SHADER:
                return GL_VERTEX_SHADER;
                break;
            case ShaderType::TESS_CONTROL_SHADER:
                return GL_TESS_CONTROL_SHADER;
                break;
            case ShaderType::TESS_EVALUATION_SHADER:
                return GL_TESS_EVALUATION_SHADER;
                break;
            case ShaderType::GEOMETRY_SHADER:
                return GL_GEOMETRY_SHADER;
                break;
            case ShaderType::FRAGMENT_SHADER:
                return GL_FRAGMENT_SHADER;
                break;
        }

        VXM_CORE_ASSERT(false, "ShaderType {0} not supported in OpenGL.", (int)shaderType);
        return 0;
    }

    std::string ShaderTypeToString(ShaderType shaderType){
        VXM_PROFILE_FUNCTION();

        switch (shaderType) {
            case ShaderType::COMPUTE_SHADER:
                return COMPUTE_TYPE;
                break;
            case ShaderType::VERTEX_SHADER:
                return VERTEX_TYPE;
                break;
            case ShaderType::TESS_CONTROL_SHADER:
                return "__TYPE_TESS_CONTROL_SHADER__";
                break;
            case ShaderType::TESS_EVALUATION_SHADER:
                return "__TYPE_TESS_EVALUATION_SHADER__";
                break;
            case ShaderType::GEOMETRY_SHADER:
                return GEOMETRY_TYPE;
                break;
            case ShaderType::FRAGMENT_SHADER:
                return FRAGMENT_TYPE;
                break;
        }
        return "UNKNOWN";
    }

    ShaderType ShaderTypeFromString(std::string type)
    {
        VXM_PROFILE_FUNCTION();
        if(type == (VERTEX_TYPE)) return ShaderType::VERTEX_SHADER;
        else if(type == (FRAGMENT_TYPE) || type == PIXEL_TYPE) return ShaderType::FRAGMENT_SHADER;
        else if(type == (GEOMETRY_TYPE)) return ShaderType::GEOMETRY_SHADER;
        else if(type == (COMPUTE_TYPE)) return ShaderType::COMPUTE_SHADER;
        else if(type == (TESS_CONTROL_SHADER_TYPE)) return ShaderType::TESS_CONTROL_SHADER;
        else if(type == (TESS_EVALUATION_SHADER_TYPE)) return ShaderType::TESS_EVALUATION_SHADER;
        VXM_CORE_ASSERT(false, "Type {0} unknown.", type);
        return ShaderType::None;
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& path) : m_Name(name)
    {
        VXM_PROFILE_FUNCTION();
        auto shaderSources = PreProcess(path);
        Compile(shaderSources);
    }

    OpenGLShader::OpenGLShader(const std::string& path)
    {
        VXM_PROFILE_FUNCTION();
        std::filesystem::path p(path);
        m_Name = p.stem().string();

        auto shaderSources = PreProcess(path);
        Compile(shaderSources);
        FillUniforms();
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& srcVertex, const std::string& srcFragment) : m_Name(name)
    {
        VXM_PROFILE_FUNCTION();
        m_Uniforms = GetAllUniform(srcVertex);
        auto uFrag = GetAllUniform(srcFragment);
        for (auto&& kp : uFrag) {
            if(!m_Uniforms.contains(kp.first)) m_Uniforms[kp.first] = -1;
        }

        Compile({
                        {ShaderType::VERTEX_SHADER,   srcVertex},
                        {ShaderType::FRAGMENT_SHADER, srcFragment},
                });
        FillUniforms();
    }

    void OpenGLShader::Compile(const std::unordered_map<ShaderType, std::string>& shaders)
    {
        VXM_PROFILE_FUNCTION();

        unsigned int program;
        {
            VXM_PROFILE_SCOPE("OpenGLShader::Compile -> Create Shader Program");
            program = glCreateProgram();
            glObjectLabel(GL_PROGRAM, program, static_cast<GLsizei>(m_Name.size()), m_Name.c_str());
        }

        std::array<uint32_t, ShaderTypeCount> subShaders{};
        uint32_t offset = 0;

        for (auto& kv : shaders)
        {
            VXM_PROFILE_SCOPE("OpenGLShader::Compile -> Create and Attach Shader");
            ShaderType type = kv.first;
            const std::string& source = kv.second;

            uint32_t id = CreateSubShader(type, source);
            subShaders[offset++] = id;
            if(CompileSubShader(id)){
                glAttachShader(program, id);
            } else {
                break;
            }
        }


        if(Link(program))
        {
            m_RendererID = program;
        }
        else
        {
            VXM_PROFILE_SCOPE("OpenGLShader::Compile -> Detach shaders");
            for (int i = 0; i <= offset; ++i) {
                glDetachShader(program, subShaders[i]);
            }
        }

        {
            VXM_PROFILE_SCOPE("OpenGLShader::Compile -> Delete shaders");
            for (int i = 0; i <= offset; ++i) {
                glDeleteShader(subShaders[i]);
            }
        }
    }

    std::unordered_map<ShaderType, std::string> OpenGLShader::PreProcess(const std::string& path)
    {
        VXM_PROFILE_FUNCTION();
        std::unordered_map<ShaderType, std::string> shaderSources;

        std::string source = SystemHelper::ReadFile(path);

        m_Uniforms = GetAllUniform(source);

        std::regex regex("#define\\s*(__TYPE_\\w+)\\r?\\n", std::regex_constants::ECMAScript);

        auto words_begin = std::sregex_iterator(source.begin(), source.end(), regex);
        auto words_end = std::sregex_iterator();
        auto count = std::distance(words_begin, words_end);
        VXM_CORE_INFO("Found {0} shaders.", count);

        for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
            auto next = i; ++next;

            std::smatch match = *i;

            const std::string& type = match[1].str();

            int begin = static_cast<int>(match.position() + match.length());
            int end = static_cast<int>(next != words_end ? next->position() : source.size());
            shaderSources[ShaderTypeFromString(type)] = source.substr(begin, end - begin);
        }

        return shaderSources;
    }

    std::unordered_map<std::string, int> OpenGLShader::GetAllUniform(const std::string& source){
        VXM_PROFILE_FUNCTION();
        std::unordered_map<std::string, int> uniforms;

        std::regex regex(".*uniform\\s+(\\w+)\\s+(\\w+)(\\s*\\[\\s*(\\w+)\\s*\\])?", std::regex_constants::ECMAScript);
        std::regex numberRegex("\\d+", std::regex_constants::ECMAScript);
        auto uniformBegin = std::sregex_iterator(source.begin(), source.end(), regex);
        auto uniformEnd = std::sregex_iterator();
        auto count = std::distance(uniformBegin, uniformEnd);
        uniforms.reserve(count);

        for (std::sregex_iterator i = uniformBegin; i != uniformEnd; ++i) {
            auto next = i; ++next;

            std::smatch match = *i;

            std::string name = match[1].str();
            if(!uniforms.contains(name)) {
                uniforms[name] = -1;
            }
        }

        return uniforms;
    }

    OpenGLShader::~OpenGLShader(){
        VXM_PROFILE_FUNCTION();
        glDeleteProgram(m_RendererID);
    }

    bool OpenGLShader::Link() {
        return Link(m_RendererID);
    }

    bool OpenGLShader::Link(unsigned int rendererId) {
        VXM_PROFILE_FUNCTION();
        // Link our program
        glLinkProgram(rendererId);

        GLint isLinked = 0;
        glGetProgramiv(rendererId, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(rendererId, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(rendererId, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(rendererId);

            // Use the infoLog as you see fit.
            VXM_CORE_ERROR("{0}", infoLog.data());
            VXM_CORE_ASSERT(false, "Linking of shader program failed.");

            // In this simple program, we'll just leave
            return false;
        }
        return true;
    }

    void OpenGLShader::Bind() const {
        VXM_PROFILE_FUNCTION();
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const {
        VXM_PROFILE_FUNCTION();
        glUseProgram(0);
    }

    void OpenGLShader::SetUniformInt(const std::string& name, int value){
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name];
        glUniform1i(location, value);
    }

    void OpenGLShader::SetUniformFloat(const std::string& name, float value){
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name];
        glUniform1f(location, value);
    }

    void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value){
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name];
        glUniform2fv(location, 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value){
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name];
        glUniform3fv(location, 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value){
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name];
        glUniform4fv(location, 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& value) {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name];
        glUniformMatrix3fv(location, 1, false, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& value) {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name];
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
    }

    uint32_t OpenGLShader::CreateSubShader(ShaderType type, const std::string &source) {
        VXM_PROFILE_FUNCTION();
        auto id = glCreateShader(GetShaderTypeID(type));
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        return id;
    }

    bool OpenGLShader::CompileSubShader(uint32_t id) {
        VXM_PROFILE_FUNCTION();
        // Compile the vertex shader
        glCompileShader(id);
        GLint isCompiled = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(id);

            // Use the infoLog as you see fit.
            VXM_CORE_ERROR("{0}", infoLog.data());
            VXM_CORE_ASSERT(false, "Compiling of shader failed.");
            // In this simple program, we'll just leave

            return false;
        }
        return true;
    }

    void OpenGLShader::DeleteSubShader(uint32_t id) {
        VXM_PROFILE_FUNCTION();
        glDeleteShader(m_RendererID);
    }

    std::vector<std::string> OpenGLShader::GetUniforms() const {
        VXM_PROFILE_FUNCTION();
        std::vector<std::string> uniforms;
        uniforms.reserve(m_Uniforms.size());
        for (auto&& kp : m_Uniforms) {
            uniforms.push_back(kp.first);
        }
        return uniforms;
    }

    void OpenGLShader::FillUniforms() {
        VXM_PROFILE_FUNCTION();
        for (auto&& kv : m_Uniforms) {
            m_Uniforms[kv.first] = glGetUniformLocation(m_RendererID, kv.first.c_str());
        }
    }

} // Core