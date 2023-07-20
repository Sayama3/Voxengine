//
// Created by ianpo on 20/07/2023.
//

#include "OpenGLShader.hpp"
#include "Voxymore/Logger.hpp"
#include <glad/glad.h>


namespace Voxymore::Core {
    unsigned int GetShaderTypeID(ShaderType shaderType) {
        switch (shaderType) {
            case COMPUTE_SHADER:
                return GL_COMPUTE_SHADER;
                break;
            case VERTEX_SHADER:
                return GL_VERTEX_SHADER;
                break;
            case TESS_CONTROL_SHADER:
                return GL_TESS_CONTROL_SHADER;
                break;
            case TESS_EVALUATION_SHADER:
                return GL_TESS_EVALUATION_SHADER;
                break;
            case GEOMETRY_SHADER:
                return GL_GEOMETRY_SHADER;
                break;
            case FRAGMENT_SHADER:
                return GL_FRAGMENT_SHADER;
                break;
        }

        VXM_CORE_ERROR("ShaderType {0} not supported in OpenGL.", ShaderTypeToString(shaderType));
        return 0;
    }

    OpenGLSubShader::OpenGLSubShader(const ShaderParams& shader) : m_Type(shader.type), m_RendererID(glCreateShader(GetType())) {
        const char* src = shader.source.c_str();
        glShaderSource(GetId(), 1, &src, nullptr);
    }

    OpenGLSubShader::OpenGLSubShader(const std::string &source, ShaderType shaderType) : m_Type(shaderType),  m_RendererID(glCreateShader(GetType())) {
        const char* src = source.c_str();
        glShaderSource(GetId(), 1, &src, nullptr);
    }

    OpenGLSubShader::~OpenGLSubShader() {
        glDeleteShader(m_RendererID);
    }

    bool OpenGLSubShader::Compile() const {
        // Compile the vertex shader
        glCompileShader(m_RendererID);
        GLint isCompiled = 0;
        glGetShaderiv(m_RendererID, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(m_RendererID);

            // Use the infoLog as you see fit.
            VXM_CORE_ERROR("Compiling of shader {0} failed.", ShaderTypeToString(m_Type));
            VXM_CORE_ERROR("{0}", infoLog.data());
            // In this simple program, we'll just leave
            return false;
        }
        return true;
    }

#define ADD_SUB_SHADER(name, shader) OpenGLSubShader name(shader); \
                                name.Compile(); \
                                this->Attach(name)


    OpenGLShader::OpenGLShader(const ShaderParams &shader1) : m_RendererID(glCreateProgram()), m_ShaderProgramType(shader1.type) {
        ADD_SUB_SHADER(s1, shader1);
        Link();
    }

    OpenGLShader::OpenGLShader(const ShaderParams &shader1, const ShaderParams &shader2) : m_RendererID(glCreateProgram()), m_ShaderProgramType((ShaderType)((int)shader1.type | (int)shader2.type)) {
        ADD_SUB_SHADER(s1, shader1);
        ADD_SUB_SHADER(s2, shader2);
        Link();

    }

    OpenGLShader::OpenGLShader(const ShaderParams &shader1, const ShaderParams &shader2, const ShaderParams &shader3) : m_RendererID(glCreateProgram()), m_ShaderProgramType((ShaderType)((int)shader1.type | (int)shader2.type | (int)shader3.type)) {
        ADD_SUB_SHADER(s1, shader1);
        ADD_SUB_SHADER(s2, shader2);
        ADD_SUB_SHADER(s3, shader3);
        Link();
    }


    void OpenGLShader::Attach(const OpenGLSubShader& subShader) const {
        glAttachShader(m_RendererID, subShader.GetId());
    }

    void OpenGLShader::Link() const {// Note the different functions here: glGetProgram* instead of glGetShader*.
        // Link our program
        glLinkProgram(m_RendererID);

        GLint isLinked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_RendererID);

            // Use the infoLog as you see fit.
            VXM_CORE_ERROR("Linking of shader program failed.");
            VXM_CORE_ERROR("{0}", infoLog.data());

            // In this simple program, we'll just leave
            return;
        }
    }

    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const {
        glUseProgram(0);
    }

    bool OpenGLShader::HasType(ShaderType shaderType) const {
        return this->m_ShaderProgramType & shaderType;
    }

} // Core