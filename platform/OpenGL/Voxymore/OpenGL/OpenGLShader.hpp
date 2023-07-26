//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Math.hpp"

namespace Voxymore::Core {
    enum OpenGLShaderType {
        COMPUTE_SHADER = 0,
        VERTEX_SHADER = 1,
        TESS_CONTROL_SHADER = 2,
        TESS_EVALUATION_SHADER = 3,
        GEOMETRY_SHADER = 4,
        FRAGMENT_SHADER = 5
    };


    class OpenGLSubShader {
    public:
        OpenGLSubShader(const std::string& src, OpenGLShaderType shaderType);
        ~OpenGLSubShader();

        OpenGLSubShader (const OpenGLSubShader&) = delete;
        OpenGLSubShader& operator= (const OpenGLSubShader&) = delete;

        bool Compile();
        uint32_t GetID();
    private:
        OpenGLShaderType m_Type;
        uint32_t m_RendererID;
    };

    class OpenGLShader : public Shader {
    public:
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
        void Link();
        unsigned int m_RendererID;
    };

} // Core
