//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core/SystemHelper.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Renderer/Shader.hpp"

namespace Voxymore::Core {
    // TODO: Analyse shaders to get the different uniform inside.
    class OpenGLShader : public Shader {
	private:
		enum Target
		{
			None,
			Vulkan,
			OpenGl,
			HashVulkan,
			HashOpenGl,
		};
    public:
        OpenGLShader(const std::string& name, const Path& path);
        OpenGLShader(const Path& path);
        OpenGLShader(const std::string& name, const Path& vertexPath, const Path& fragmentPath);
        OpenGLShader(const Path& vertexPath, const Path& fragmentPath);
        OpenGLShader(std::unordered_map<ShaderType, Path> paths);
        OpenGLShader(const std::string& name, std::unordered_map<ShaderType, Path> paths);
        OpenGLShader(const std::string& name, const std::string& srcVertex, const std::string& srcFragment);
        virtual ~OpenGLShader() override;

        OpenGLShader (const OpenGLShader&) = delete;
        OpenGLShader& operator= (const OpenGLShader&) = delete;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        inline virtual const std::string& GetName() const override { return m_Name; }

//        virtual void SetUniform(const std::string& name, const void* valuePtr, uint32_t size) override;
		virtual void SetUniformInt(const std::string& name, int value) override;
		virtual void SetUniformInt2(const std::string& name, const glm::ivec2& value) override;
		virtual void SetUniformInt3(const std::string& name, const glm::ivec3& value) override;
		virtual void SetUniformInt4(const std::string& name, const glm::ivec4& value) override;

		virtual void SetUniformFloat(const std::string& name, float value) override;
		virtual void SetUniformFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& value) override;

		virtual void SetUniformMat2(const std::string& name, const glm::mat2& value) override;
		virtual void SetUniformMat3(const std::string& name, const glm::mat3& value) override;
		virtual void SetUniformMat4(const std::string& name, const glm::mat4& value) override;

		virtual void SetUniformBool(const std::string& name, const bool& value) override;
		virtual void SetUniformBool2(const std::string& name, const glm::bvec2& value) override;
		virtual void SetUniformBool3(const std::string& name, const glm::bvec3& value) override;
		virtual void SetUniformBool4(const std::string& name, const glm::bvec4& value) override;

		virtual void SetUniformSampler1D(const std::string& name, const uint32_t& value) override;
		virtual void SetUniformSampler2D(const std::string& name, const uint32_t& value) override;
		virtual void SetUniformSampler3D(const std::string& name, const uint32_t& value) override;

		virtual void Reload() override;
		virtual bool ShouldReload() const override;
	private:
        std::unordered_map<ShaderType, std::string> PreProcess(const Path& path);

		void CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& shaders);
		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void Reflect(ShaderType stage, const std::vector<uint32_t>& shaderData);
		void DeleteProgram();

		Path GetCachePath(ShaderType shaderType, Target target) const;
    private:
		std::unordered_map<ShaderType, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<ShaderType, std::vector<uint32_t>> m_OpenGLSPIRV;

//		std::unordered_map<ShaderType, bool> m_HashesChanged;

		std::unordered_map<ShaderType, std::string> m_OpenGLSourceCode;
    private:
//		Path m_FilePath;
		std::unordered_map<ShaderType, Path> m_FilePaths;
        std::string m_Name;
        unsigned int m_RendererID;
    };



} // Core
