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
    ShaderDataType GetShaderDataType(GLenum type)
    {
        switch (type) {
            case (GL_FLOAT): return ShaderDataType::Float; // float
            case (GL_FLOAT_VEC2): return ShaderDataType::Float2; // vec2
            case (GL_FLOAT_VEC3): return ShaderDataType::Float3; // vec3
            case (GL_FLOAT_VEC4): return ShaderDataType::Float4; // vec4
//            case (GL_DOUBLE): return ShaderDataType::Double; // double
//            case (GL_DOUBLE_VEC2): return ShaderDataType::Double2; // dvec2
//            case (GL_DOUBLE_VEC3): return ShaderDataType::Double3; // dvec3
//            case (GL_DOUBLE_VEC4): return ShaderDataType::Double4; // dvec4
            case (GL_INT): return ShaderDataType::Int; // int
            case (GL_INT_VEC2): return ShaderDataType::Int2; // ivec2
            case (GL_INT_VEC3): return ShaderDataType::Int3; // ivec3
            case (GL_INT_VEC4): return ShaderDataType::Int4; // ivec4
            case (GL_UNSIGNED_INT): return ShaderDataType::UInt; // unsigned int
            case (GL_UNSIGNED_INT_VEC2): return ShaderDataType::UInt2; // uvec2
            case (GL_UNSIGNED_INT_VEC3): return ShaderDataType::UInt3; // uvec3
            case (GL_UNSIGNED_INT_VEC4): return ShaderDataType::UInt4; // uvec4
            case (GL_BOOL): return ShaderDataType::Bool; // bool
            case (GL_BOOL_VEC2): return ShaderDataType::Bool2; // bvec2
            case (GL_BOOL_VEC3): return ShaderDataType::Bool3; // bvec3
            case (GL_BOOL_VEC4): return ShaderDataType::Bool4; // bvec4
            case (GL_FLOAT_MAT2): return ShaderDataType::Mat2; // mat2
            case (GL_FLOAT_MAT3): return ShaderDataType::Mat3; // mat3
            case (GL_FLOAT_MAT4): return ShaderDataType::Mat4; // mat4
//            case (GL_FLOAT_MAT2x3): return ShaderDataType::Mat2x3; // mat2x3
//            case (GL_FLOAT_MAT2x4): return ShaderDataType::Mat2x4; // mat2x4
//            case (GL_FLOAT_MAT3x2): return ShaderDataType::Mat3x2; // mat3x2
//            case (GL_FLOAT_MAT3x4): return ShaderDataType::Mat3x4; // mat3x4
//            case (GL_FLOAT_MAT4x2): return ShaderDataType::Mat4x2; // mat4x2
//            case (GL_FLOAT_MAT4x3): return ShaderDataType::Mat4x3; // mat4x3
//            case (GL_DOUBLE_MAT2): return ShaderDataType::DMat2; // dmat2
//            case (GL_DOUBLE_MAT3): return ShaderDataType::DMat3; // dmat3
//            case (GL_DOUBLE_MAT4): return ShaderDataType::DMat4; // dmat4
//            case (GL_DOUBLE_MAT2x3): return ShaderDataType::DMat2x3; // dmat2x3
//            case (GL_DOUBLE_MAT2x4): return ShaderDataType::DMat2x4; // dmat2x4
//            case (GL_DOUBLE_MAT3x2): return ShaderDataType::DMat3x2; // dmat3x2
//            case (GL_DOUBLE_MAT3x4): return ShaderDataType::DMat3x4; // dmat3x4
//            case (GL_DOUBLE_MAT4x2): return ShaderDataType::DMat4x2; // dmat4x2
//            case (GL_DOUBLE_MAT4x3): return ShaderDataType::DMat4x3; // dmat4x3
            case (GL_SAMPLER_1D): return ShaderDataType::Sampler1D; // sampler1D
            case (GL_SAMPLER_2D): return ShaderDataType::Sampler2D; // sampler2D
            case (GL_SAMPLER_3D): return ShaderDataType::Sampler3D; // sampler3D
//            case (GL_SAMPLER_CUBE): return ShaderDataType::Sampler_Cube; // samplerCube
//            case (GL_SAMPLER_1D_SHADOW): return ShaderDataType::SAMPLER_1D_SHADOW; // sampler1DShadow
//            case (GL_SAMPLER_2D_SHADOW): return ShaderDataType::SAMPLER_2D_SHADOW; // sampler2DShadow
//            case (GL_SAMPLER_1D_ARRAY): return ShaderDataType::SAMPLER_1D_ARRAY; // sampler1DArray
//            case (GL_SAMPLER_2D_ARRAY): return ShaderDataType::SAMPLER_2D_ARRAY; // sampler2DArray
//            case (GL_SAMPLER_1D_ARRAY_SHADOW): return ShaderDataType::SAMPLER_1D_ARRAY_SHADOW; // sampler1DArrayShadow
//            case (GL_SAMPLER_2D_ARRAY_SHADOW): return ShaderDataType::SAMPLER_2D_ARRAY_SHADOW; // sampler2DArrayShadow
//            case (GL_SAMPLER_2D_MULTISAMPLE): return ShaderDataType::SAMPLER_2D_MULTISAMPLE; // sampler2DMS
//            case (GL_SAMPLER_2D_MULTISAMPLE_ARRAY): return ShaderDataType::SAMPLER_2D_MULTISAMPLE_ARRAY; // sampler2DMSArray
//            case (GL_SAMPLER_CUBE_SHADOW): return ShaderDataType::SAMPLER_CUBE_SHADOW; // samplerCubeShadow
//            case (GL_SAMPLER_BUFFER): return ShaderDataType::SAMPLER_BUFFER; // samplerBuffer
//            case (GL_SAMPLER_2D_RECT): return ShaderDataType::SAMPLER_2D_RECT; // sampler2DRect
//            case (GL_SAMPLER_2D_RECT_SHADOW): return ShaderDataType::SAMPLER_2D_RECT_SHADOW; // sampler2DRectShadow
//            case (GL_INT_SAMPLER_1D): return ShaderDataType::INT_SAMPLER_1D; // isampler1D
//            case (GL_INT_SAMPLER_2D): return ShaderDataType::INT_SAMPLER_2D; // isampler2D
//            case (GL_INT_SAMPLER_3D): return ShaderDataType::INT_SAMPLER_3D; // isampler3D
//            case (GL_INT_SAMPLER_CUBE): return ShaderDataType::INT_SAMPLER_CUBE; // isamplerCube
//            case (GL_INT_SAMPLER_1D_ARRAY): return ShaderDataType::INT_SAMPLER_1D_ARRAY; // isampler1DArray
//            case (GL_INT_SAMPLER_2D_ARRAY): return ShaderDataType::INT_SAMPLER_2D_ARRAY; // isampler2DArray
//            case (GL_INT_SAMPLER_2D_MULTISAMPLE): return ShaderDataType::INT_SAMPLER_2D_MULTISAMPLE; // isampler2DMS
//            case (GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY): return ShaderDataType::INT_SAMPLER_2D_MULTISAMPLE_ARRAY; // isampler2DMSArray
//            case (GL_INT_SAMPLER_BUFFER): return ShaderDataType::INT_SAMPLER_BUFFER; // isamplerBuffer
//            case (GL_INT_SAMPLER_2D_RECT): return ShaderDataType::INT_SAMPLER_2D_RECT; // isampler2DRect
//            case (GL_UNSIGNED_INT_SAMPLER_1D): return ShaderDataType::UNSIGNED_INT_SAMPLER_1D; // usampler1D
//            case (GL_UNSIGNED_INT_SAMPLER_2D): return ShaderDataType::UNSIGNED_INT_SAMPLER_2D; // usampler2D
//            case (GL_UNSIGNED_INT_SAMPLER_3D): return ShaderDataType::UNSIGNED_INT_SAMPLER_3D; // usampler3D
//            case (GL_UNSIGNED_INT_SAMPLER_CUBE): return ShaderDataType::UNSIGNED_INT_SAMPLER_CUBE; // usamplerCube
//            case (GL_UNSIGNED_INT_SAMPLER_1D_ARRAY): return ShaderDataType::UNSIGNED_INT_SAMPLER_1D_ARRAY; // usampler2DArray
//            case (GL_UNSIGNED_INT_SAMPLER_2D_ARRAY): return ShaderDataType::UNSIGNED_INT_SAMPLER_2D_ARRAY; // usampler2DArray
//            case (GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE): return ShaderDataType::UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE; // usampler2DMS
//            case (GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY): return ShaderDataType::UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY; // usampler2DMSArray
//            case (GL_UNSIGNED_INT_SAMPLER_BUFFER): return ShaderDataType::UNSIGNED_INT_SAMPLER_BUFFER; // usamplerBuffer
//            case (GL_UNSIGNED_INT_SAMPLER_2D_RECT): return ShaderDataType::UNSIGNED_INT_SAMPLER_2D_RECT; // usampler2DRect
//            case (GL_IMAGE_1D): return ShaderDataType::IMAGE_1D; // image1D
//            case (GL_IMAGE_2D): return ShaderDataType::IMAGE_2D; // image2D
//            case (GL_IMAGE_3D): return ShaderDataType::IMAGE_3D; // image3D
//            case (GL_IMAGE_2D_RECT): return ShaderDataType::IMAGE_2D_RECT; // image2DRect
//            case (GL_IMAGE_CUBE): return ShaderDataType::IMAGE_CUBE; // imageCube
//            case (GL_IMAGE_BUFFER): return ShaderDataType::IMAGE_BUFFER; // imageBuffer
//            case (GL_IMAGE_1D_ARRAY): return ShaderDataType::IMAGE_1D_ARRAY; // image1DArray
//            case (GL_IMAGE_2D_ARRAY): return ShaderDataType::IMAGE_2D_ARRAY; // image2DArray
//            case (GL_IMAGE_2D_MULTISAMPLE): return ShaderDataType::IMAGE_2D_MULTISAMPLE; // image2DMS
//            case (GL_IMAGE_2D_MULTISAMPLE_ARRAY): return ShaderDataType::IMAGE_2D_MULTISAMPLE_ARRAY; // image2DMSArray
//            case (GL_INT_IMAGE_1D): return ShaderDataType::INT_IMAGE_1D; // iimage1D
//            case (GL_INT_IMAGE_2D): return ShaderDataType::INT_IMAGE_2D; // iimage2D
//            case (GL_INT_IMAGE_3D): return ShaderDataType::INT_IMAGE_3D; // iimage3D
//            case (GL_INT_IMAGE_2D_RECT): return ShaderDataType::INT_IMAGE_2D_RECT; // iimage2DRect
//            case (GL_INT_IMAGE_CUBE): return ShaderDataType::INT_IMAGE_CUBE; // iimageCube
//            case (GL_INT_IMAGE_BUFFER): return ShaderDataType::INT_IMAGE_BUFFER; // iimageBuffer
//            case (GL_INT_IMAGE_1D_ARRAY): return ShaderDataType::INT_IMAGE_1D_ARRAY; // iimage1DArray
//            case (GL_INT_IMAGE_2D_ARRAY): return ShaderDataType::INT_IMAGE_2D_ARRAY; // iimage2DArray
//            case (GL_INT_IMAGE_2D_MULTISAMPLE): return ShaderDataType::INT_IMAGE_2D_MULTISAMPLE; // iimage2DMS
//            case (GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY): return ShaderDataType::INT_IMAGE_2D_MULTISAMPLE_ARRAY; // iimage2DMSArray
//            case (GL_UNSIGNED_INT_IMAGE_1D): return ShaderDataType::UNSIGNED_INT_IMAGE_1D; // uimage1D
//            case (GL_UNSIGNED_INT_IMAGE_2D): return ShaderDataType::UNSIGNED_INT_IMAGE_2D; // uimage2D
//            case (GL_UNSIGNED_INT_IMAGE_3D): return ShaderDataType::UNSIGNED_INT_IMAGE_3D; // uimage3D
//            case (GL_UNSIGNED_INT_IMAGE_2D_RECT): return ShaderDataType::UNSIGNED_INT_IMAGE_2D_RECT; // uimage2DRect
//            case (GL_UNSIGNED_INT_IMAGE_CUBE): return ShaderDataType::UNSIGNED_INT_IMAGE_CUBE; // uimageCube
//            case (GL_UNSIGNED_INT_IMAGE_BUFFER): return ShaderDataType::UNSIGNED_INT_IMAGE_BUFFER; // uimageBuffer
//            case (GL_UNSIGNED_INT_IMAGE_1D_ARRAY): return ShaderDataType::UNSIGNED_INT_IMAGE_1D_ARRAY; // uimage1DArray
//            case (GL_UNSIGNED_INT_IMAGE_2D_ARRAY): return ShaderDataType::UNSIGNED_INT_IMAGE_2D_ARRAY; // uimage2DArray
//            case (GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE): return ShaderDataType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE; // uimage2DMS
//            case (GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY): return ShaderDataType::UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY; // uimage2DMSArray
//            case (GL_UNSIGNED_INT_ATOMIC_COUNTER): return ShaderDataType::UNSIGNED_INT_ATOMIC_COUNTER; // atomic_uint
            default : return ShaderDataType::None;
        }
    }

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
        if (Compile(shaderSources))
        {
            m_Uniforms = GetAllUniform(m_RendererID);
        }
    }

    OpenGLShader::OpenGLShader(const std::string& path)
    {
        VXM_PROFILE_FUNCTION();
        std::filesystem::path p(path);
        m_Name = p.stem().string();

        auto shaderSources = PreProcess(path);
        if (Compile(shaderSources))
        {
            m_Uniforms = GetAllUniform(m_RendererID);
        }
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& srcVertex, const std::string& srcFragment) : m_Name(name)
    {
        VXM_PROFILE_FUNCTION();
//        m_Uniforms = GetAllUniform(srcVertex);
//        auto uFrag = GetAllUniform(srcFragment);
//        for (auto&& kp : uFrag) {
//            if(!m_Uniforms.contains(kp.first)) m_Uniforms[kp.first] = -1;
//        }

        if (Compile({
                        {ShaderType::VERTEX_SHADER,   srcVertex},
                        {ShaderType::FRAGMENT_SHADER, srcFragment},
                }))
        {
            m_Uniforms = GetAllUniform(m_RendererID);
        }
    }

    bool OpenGLShader::Compile(const std::unordered_map<ShaderType, std::string>& shaders)
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

        bool linked = Link(program);
        if(linked)
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

        return linked;
    }

    std::unordered_map<ShaderType, std::string> OpenGLShader::PreProcess(const std::string& path)
    {
        VXM_PROFILE_FUNCTION();
        std::unordered_map<ShaderType, std::string> shaderSources;

        std::string source = SystemHelper::ReadFile(path);

//        m_Uniforms = GetAllUniform(source);

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

    std::unordered_map<std::string, OpenGLUniformDescription> OpenGLShader::GetAllUniform(uint32_t program){
        VXM_PROFILE_FUNCTION();
        std::unordered_map<std::string, OpenGLUniformDescription> uniforms;

        int i;
        int count;

        int size; // size of the variable
        GLenum type; // type of the variable (float, vec3 or mat4, etc)

        const GLsizei bufSize = 256; // maximum name length
        char name[bufSize]; // variable name in GLSL
        GLsizei length; // name length
        glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
        printf("Active Uniforms: %d\n", count);
        uniforms.reserve(count);

        for (i = 0; i < count; i++)
        {
            glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);

            std::string s_name(name, length);

            uniforms.insert({s_name,OpenGLUniformDescription(s_name, GetShaderDataType(type), size, glGetUniformLocation(program, s_name.c_str()))});

            VXM_CORE_INFO("Uniform #%d Type: %u Name: %s\n", i, type, name);
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

    void OpenGLShader::SetUniform(const std::string& name, const void* valuePtr, uint32_t size){
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        VXM_CORE_ASSERT(m_Uniforms[name].Size == size, "The value size doesn't match the size found of the uniform.", name);
        switch (m_Uniforms[name].Type) {
            case ShaderDataType::Float: SetUniformFloat(name, *(float*)valuePtr); break;
            case ShaderDataType::Float2: SetUniformFloat2(name, *(glm::vec2*)valuePtr); break;
            case ShaderDataType::Float3: SetUniformFloat3(name, *(glm::vec3*)valuePtr); break;
            case ShaderDataType::Float4: SetUniformFloat4(name, *(glm::vec4*)valuePtr); break;
//            case ShaderDataType::Mat2: SetUniformMat2(name, *(glm::mat2*)valuePtr); break;
            case ShaderDataType::Mat3: SetUniformMat3(name, *(glm::mat3*)valuePtr); break;
            case ShaderDataType::Mat4: SetUniformMat4(name, *(glm::mat4*)valuePtr); break;
            case ShaderDataType::Int: SetUniformInt(name, *(int*)valuePtr); break;
//            case ShaderDataType::Int2: SetUniformInt2(name, *(glm::ivec2*)valuePtr); break;
//            case ShaderDataType::Int3: SetUniformInt3(name, *(glm::ivec3*)valuePtr); break;
//            case ShaderDataType::Int4: SetUniformInt4(name, *(glm::ivec4*)valuePtr); break;
//            case ShaderDataType::UInt: SetUniformUInt(name, *(uint32_t *)valuePtr); break;
//            case ShaderDataType::UInt2: SetUniformUInt2(name, *(glm::uvec2*)valuePtr); break;
//            case ShaderDataType::UInt3: SetUniformUInt3(name, *(glm::uvec3*)valuePtr); break;
//            case ShaderDataType::UInt4: SetUniformUInt4(name, *(glm::uvec4*)valuePtr); break;
//            case ShaderDataType::Bool: SetUniformBool(name, *(bool*)valuePtr); break;
//            case ShaderDataType::Bool2: SetUniformBool2(name, *(glm::bool2*)valuePtr); break;
//            case ShaderDataType::Bool3: SetUniformBool3(name, *(glm::bool3*)valuePtr); break;
//            case ShaderDataType::Bool4: SetUniformBool4(name, *(glm::bool4*)valuePtr); break;
            case ShaderDataType::Sampler1D: SetUniformInt(name, *(int*)valuePtr); break;
            case ShaderDataType::Sampler2D: SetUniformInt(name, *(int*)valuePtr); break;
            case ShaderDataType::Sampler3D: SetUniformInt(name, *(int*)valuePtr); break;
        }
        VXM_CORE_ASSERT(false, "The ShaderDataType '{0}' is currently not handled by OpenGLShader.", ShaderTypeToString(m_Uniforms[name].Type));
    }

    void OpenGLShader::SetUniformInt(const std::string& name, int value){
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name].Location;
        glUniform1i(location, value);
    }

    void OpenGLShader::SetUniformFloat(const std::string& name, float value){
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name].Location;
        glUniform1f(location, value);
    }

    void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value){
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name].Location;
        glUniform2fv(location, 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value){
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name].Location;
        glUniform3fv(location, 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value){
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name].Location;
        glUniform4fv(location, 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& value) {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name].Location;
        glUniformMatrix3fv(location, 1, false, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& value) {
        VXM_PROFILE_FUNCTION();
        VXM_CORE_ASSERT(m_Uniforms.contains(name), "The uniform map doesn't contains the uniform '{0}'.", name);
        int location = m_Uniforms[name].Location;
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

    std::unordered_map<std::string, UniformDescription> OpenGLShader::GetUniforms() const {
        VXM_PROFILE_FUNCTION();
        std::unordered_map<std::string, UniformDescription> uniforms;
        uniforms.reserve(m_Uniforms.size());

        for (std::pair<std::string, OpenGLUniformDescription> && kp : m_Uniforms) {
            uniforms[kp.first] = kp.second;
        }

        return uniforms;
    }

} // Core