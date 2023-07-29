//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Math.hpp"
#include "Voxymore/SystemHelper.hpp"

namespace Voxymore::Core {
    class OpenGLSubShader {
    public:
        OpenGLSubShader(const std::string& src, ShaderType shaderType);
        ~OpenGLSubShader();

        OpenGLSubShader (const OpenGLSubShader&) = delete;
        OpenGLSubShader& operator= (const OpenGLSubShader&) = delete;

        bool Compile();
        uint32_t GetID();
    private:
        ShaderType m_Type;
        uint32_t m_RendererID;
    };

    // TODO: Analyse shaders to get the different uniform inside.
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::vector<std::string>& paths);
        OpenGLShader(const std::unordered_map<ShaderType, std::string>& paths);
        OpenGLShader(const std::string& srcVertex, const std::string& srcFragment);
        virtual ~OpenGLShader() override;

        OpenGLShader (const OpenGLShader&) = delete;
        OpenGLShader& operator= (const OpenGLShader&) = delete;

        virtual void Bind() const override;
        virtual void Unbind() const override;



		void SetUniformInt(const std::string& name, int value);

		void SetUniformFloat(const std::string& name, float value);
		void SetUniformFloat2(const std::string& name, const glm::vec2& value);
		void SetUniformFloat3(const std::string& name, const glm::vec3& value);
		void SetUniformFloat4(const std::string& name, const glm::vec4& value);

		void SetUniformMat3(const std::string& name, const glm::mat3& value);
		void SetUniformMat4(const std::string& name, const glm::mat4& value);
	private:
        std::unordered_map<ShaderType, std::string> PreProcess(const std::vector<std::string>& paths);
        void Compile(std::unordered_map<ShaderType, std::string> shaders);
        bool Link();
        bool Link(unsigned int rendererId);
    private:
        unsigned int m_RendererID;
    };

} // Core
