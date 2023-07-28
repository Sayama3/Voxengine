//
// Created by ianpo on 20/07/2023.
//

#include "OpenGLShader.hpp"
#include "Voxymore/Core.hpp"
#include "Voxymore/Logger.hpp"
#include <glad/glad.h>

#ifdef _WIN32
#define NEWLINE "\r\n"
#elif defined macintosh // OS 9
#define NEWLINE "\r"
#else
    #define NEWLINE "\n" // Mac OS X uses \n
#endif

#define SHADER_DEFINE_TYPE "#define  __TYPE_"
#define VERTEX_TYPE "VERTEX_SHADER__"
#define FRAGMENT_TYPE "FRAGMENT_SHADER__"
#define PIXEL_TYPE "PIXEL_SHADER__"
#define GEOMETRY_TYPE "GEOMETRY_SHADER__"
#define COMPUTE_TYPE "COMPUTE_SHADER__"


namespace Voxymore::Core {
    unsigned int GetShaderTypeID(OpenGLShaderType shaderType) {
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

        VXM_CORE_ASSERT(false, "ShaderType {0} not supported in OpenGL.", shaderType);
        return 0;
    }

    std::string ShaderTypeToString(OpenGLShaderType shaderType){

        switch (shaderType) {
            case COMPUTE_SHADER:
                return "COMPUTE_SHADER";
                break;
            case VERTEX_SHADER:
                return "VERTEX_SHADER";
                break;
            case TESS_CONTROL_SHADER:
                return "TESS_CONTROL_SHADER";
                break;
            case TESS_EVALUATION_SHADER:
                return "TESS_EVALUATION_SHADER";
                break;
            case GEOMETRY_SHADER:
                return "GEOMETRY_SHADER";
                break;
            case FRAGMENT_SHADER:
                return "FRAGMENT_SHADER";
                break;
        }
        return "UNKNOWN";
    }

    GLenum ShaderTypeFromString(std::string type)
    {
        if(type == VERTEX_TYPE) return GL_VERTEX_SHADER;
        else if(type == FRAGMENT_TYPE || type == PIXEL_TYPE) return GL_FRAGMENT_SHADER;
        else if(type == GEOMETRY_TYPE) return GL_GEOMETRY_SHADER;
        else if(type == COMPUTE_TYPE) return GL_COMPUTE_SHADER;
        return 0;
    }


    OpenGLSubShader::OpenGLSubShader(const std::string &source, OpenGLShaderType shaderType) : m_RendererID(glCreateShader(GetShaderTypeID(shaderType))) {
        const char* src = source.c_str();
        glShaderSource(m_RendererID, 1, &src, nullptr);
    }

    OpenGLSubShader::~OpenGLSubShader() {
        glDeleteShader(m_RendererID);
    }

    bool OpenGLSubShader::Compile() {
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

    uint32_t OpenGLSubShader::GetID() {
        return m_RendererID;
    }


    OpenGLShader::OpenGLShader(const std::initializer_list<std::string>& paths) : m_RendererID(glCreateProgram())
    {
        std::vector<std::string> str;
        PreProcess(str);
        Link();
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::vector<std::string>& paths)
    {
        std::unordered_map<unsigned int, std::string> shaderSources;
        size_t typeTokenLength = strlen(SHADER_DEFINE_TYPE);

        for (const auto& path : paths) {
            std::string source = SystemHelper::ReadFile(path);
            size_t pos = source.find(SHADER_DEFINE_TYPE, 0);
            while(pos != std::string::npos)
            {

                size_t eol = source.find_first_of(NEWLINE, pos);
                VXM_CORE_ASSERT(eol != std::string::npos, "Syntax error.");
                size_t begin = pos + typeTokenLength;
                std::string type = source.substr(begin, eol - begin);
                VXM_CORE_ASSERT(ShaderTypeFromString(type), "Type '{0}' not supported...", type);

                size_t beginSource = pos;
                size_t nextLinePos = source.find_first_not_of(NEWLINE, eol);
                pos = source.find(SHADER_DEFINE_TYPE, nextLinePos);
                shaderSources[ShaderTypeFromString(type)] = source.substr(beginSource, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
            }
        }

        return shaderSources;
    }

    OpenGLShader::OpenGLShader(const std::string& srcVertex, const std::string& srcFragment) : m_RendererID(glCreateProgram()) {
        OpenGLSubShader vertexShader(srcVertex, OpenGLShaderType::VERTEX_SHADER);
        vertexShader.Compile();
        glAttachShader(m_RendererID, vertexShader.GetID());

        OpenGLSubShader fragmentShader(srcFragment, OpenGLShaderType::FRAGMENT_SHADER);
        fragmentShader.Compile();
        glAttachShader(m_RendererID, fragmentShader.GetID());

        Link();
    }

    OpenGLShader::~OpenGLShader(){
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Link() {
        // Link our program
        glLinkProgram(m_RendererID);

        GLint isLinked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
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

    void OpenGLShader::Bind() const {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const {
        glUseProgram(0);
    }

    void OpenGLShader::SetUniformInt(const std::string& name, int value){
		//TODO: Precalculate every uniform and use buffer instead of setting them manually.
		const char* cname = name.c_str();
		int location = glGetUniformLocation(m_RendererID, cname);
		glUniform1i(location, value);
    }

    void OpenGLShader::SetUniformFloat(const std::string& name, float value){
		//TODO: Precalculate every uniform and use buffer instead of setting them manually.
		const char* cname = name.c_str();
		int location = glGetUniformLocation(m_RendererID, cname);
		glUniform1f(location, value);
    }

    void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value){
		//TODO: Precalculate every uniform and use buffer instead of setting them manually.
		const char* cname = name.c_str();
		int location = glGetUniformLocation(m_RendererID, cname);
		glUniform2fv(location, 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value){
		//TODO: Precalculate every uniform and use buffer instead of setting them manually.
		const char* cname = name.c_str();
		int location = glGetUniformLocation(m_RendererID, cname);
		glUniform3fv(location, 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value){
		//TODO: Precalculate every uniform and use buffer instead of setting them manually.
		const char* cname = name.c_str();
		int location = glGetUniformLocation(m_RendererID, cname);
		glUniform4fv(location, 1, glm::value_ptr(value));
    }

	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& value) {
		//TODO: Precalculate every uniform and use buffer instead of setting them manually.
		const char* cname = name.c_str();
		int location = glGetUniformLocation(m_RendererID, cname);
		glUniformMatrix3fv(location, 1, false, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& value) {
		//TODO: Precalculate every uniform and use buffer instead of setting them manually.
		const char* cname = name.c_str();
		int location = glGetUniformLocation(m_RendererID, cname);
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
	}

} // Core