//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Renderer/Shader.hpp"
#include "Voxymore/Math.hpp"

namespace Voxymore::Core {
    unsigned int GetShaderTypeID(ShaderType shaderType);

    class OpenGLSubShader {
    public:
        OpenGLSubShader(const ShaderParams& shader);
        OpenGLSubShader(const std::string& source, ShaderType shaderType);

        ~OpenGLSubShader();

        inline unsigned int GetId() const { return m_RendererID; }
        inline unsigned int GetType() const { return GetShaderTypeID(m_Type); }
        bool Compile() const;
    private:
        ShaderType m_Type;
        unsigned int m_RendererID;
    };

    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const ShaderParams& shader1);
        OpenGLShader(const ShaderParams& shader1, const ShaderParams& shader2);
        OpenGLShader(const ShaderParams& shader1, const ShaderParams& shader2, const ShaderParams& shader3);
        virtual ~OpenGLShader() override;
        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual bool HasType(ShaderType shaderType) const override;

		virtual void SetUniformMat4(const std::string& name, const glm::mat4& mat4) override;
	private:
        void Attach(const OpenGLSubShader& subShader) const;
        void Link() const;
        unsigned int m_RendererID;
        ShaderType m_ShaderProgramType;
    };

} // Core
