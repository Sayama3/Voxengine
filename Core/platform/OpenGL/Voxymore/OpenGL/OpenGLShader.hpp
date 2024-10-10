//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core/SystemHelper.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Renderer/Shader.hpp"

namespace Voxymore::Core {
    // TODO: Analyse shaders to get the different uniform inside.
    class OpenGLShader
	{
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
		OpenGLShader(const std::string& name, const std::unordered_map<ShaderType, ShaderSourceField>& sources);
        virtual ~OpenGLShader() override;

        OpenGLShader (const OpenGLShader&) = delete;
        OpenGLShader& operator= (const OpenGLShader&) = delete;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void Reload() override;
        inline virtual std::string GetName() const override { return m_Name; }
		inline virtual void SetName(const std::string& name) override { m_Name = name;}
		virtual std::vector<ShaderSourceField> GetSources() const override;
		virtual void SetSources(const std::vector<ShaderSourceField>& sources) override;
	private:
		void CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, ShaderSourceField>& shaders);
		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void Reflect(ShaderType stage, const std::vector<uint32_t>& shaderData);
		void DeleteProgram();

		Path GetCachePath(ShaderType shaderType, Target target) const;
    private:
		std::unordered_map<ShaderType, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<ShaderType, std::vector<uint32_t>> m_OpenGLSPIRV;
		std::unordered_map<ShaderType, std::string> m_OpenGLSourceCode;
    private:
		std::string m_Name;
		std::unordered_map<ShaderType, ShaderSourceField> m_Sources;
        unsigned int m_RendererID = 0;
    };

	class OpenGLGraphicsShader : public OpenGLShader, public GraphicsShader
	{
	};

	class OpenGLComputeShader : public OpenGLShader, public ComputeShader
	{
	public:
		virtual void Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ) override;
	};

} // Core
