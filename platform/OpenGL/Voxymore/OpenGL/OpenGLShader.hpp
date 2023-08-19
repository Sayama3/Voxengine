//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Core/Math.hpp"
#include "Voxymore/Core/SystemHelper.hpp"

namespace Voxymore::Core {

    struct OpenGLUniformDescription : public UniformDescription
    {
    public:
        int Location;
    public:
        OpenGLUniformDescription() = default;
        inline OpenGLUniformDescription(const std::string& name, ShaderDataType type, int size, int location) : UniformDescription(name, type, size), Location(location) {}
    };

    // TODO: Analyse shaders to get the different uniform inside.
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& name, const std::string& path);
        OpenGLShader(const std::string& path);
        OpenGLShader(const std::string& name, const std::string& srcVertex, const std::string& srcFragment);
        virtual ~OpenGLShader() override;

        OpenGLShader (const OpenGLShader&) = delete;
        OpenGLShader& operator= (const OpenGLShader&) = delete;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        inline virtual const std::string& GetName() const override { return m_Name; }

        virtual std::unordered_map<std::string, UniformDescription> GetUniforms() const override;

        virtual void SetUniform(const std::string& name, const void* valuePtr, uint32_t size) override;
		virtual void SetUniformInt(const std::string& name, int value) override;

		virtual void SetUniformFloat(const std::string& name, float value) override;
		virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) override;

		virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) override;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) override;
	private:
        std::unordered_map<ShaderType, std::string> PreProcess(const std::string& path);
        std::unordered_map<std::string, OpenGLUniformDescription> GetAllUniform(uint32_t program);

        bool Compile(const std::unordered_map<ShaderType, std::string>& shaders);
        bool Link();
        bool Link(unsigned int rendererId);
    private:
        uint32_t CreateSubShader(ShaderType type, const std::string& source);
        bool CompileSubShader(uint32_t id);
        void DeleteSubShader(uint32_t id);
    private:
        std::unordered_map<std::string, OpenGLUniformDescription> m_Uniforms;
    private:
        std::string m_Name;
        unsigned int m_RendererID;
    };



} // Core
