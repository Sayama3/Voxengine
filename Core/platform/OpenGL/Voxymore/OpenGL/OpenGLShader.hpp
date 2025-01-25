//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core/SystemHelper.hpp"
#include "Voxymore/Core/Buffer.hpp"
#include "Voxymore/Math/Math.hpp"
#include "Voxymore/Renderer/Shader.hpp"

namespace Voxymore::Core {

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
		OpenGLShader();
		~OpenGLShader();

		OpenGLShader(OpenGLShader&& other) noexcept;
		OpenGLShader& operator=(OpenGLShader&& other) noexcept;

		OpenGLShader(const OpenGLShader&) = delete;
		OpenGLShader& operator=(const OpenGLShader&) = delete;
	public:
		void swap(OpenGLShader& other);
	public:
		void SetName(std::string name);
		const std::string& GetName() const;
	public:
		void Reload();
		void AddSource(ShaderType type, ShaderSourceField shaderSource);
		void RemoveSource(ShaderType type);
		[[nodiscard]] const std::unordered_map<ShaderType, ShaderSourceField>& GetSources() const;
		[[nodiscard]] const std::unordered_map<ShaderType, std::filesystem::file_time_type>& GetSourcesTimes() const;
		void ClearSources();
	private:
		void CompileOrGetVulkanBinaries();
		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void DeleteProgram();
	private:
		[[nodiscard]] Path GetCachePath(ShaderType shaderType, Target target) const;
	public:
		[[nodiscard]] uint32_t GetRendererID() const {return m_RendererID;}
	private:
		std::unordered_map<ShaderType, ShaderSourceField> m_Sources;
		std::unordered_map<ShaderType, std::filesystem::file_time_type> m_TimeSource;
		std::unordered_map<ShaderType, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<ShaderType, std::vector<uint32_t>> m_OpenGLSPIRV;
		std::unordered_map<ShaderType, std::string> m_OpenGLSourceCode;
		std::string m_Name;
	private:
		uint32_t m_RendererID{0};
	};


    // TODO: Analyse shaders to get the different uniform inside.
    class OpenGLGraphicsShader : public Shader
	{
    public:
		OpenGLGraphicsShader(const std::string& name, const std::unordered_map<ShaderType, ShaderSourceField>& sources);
        virtual ~OpenGLGraphicsShader() override;

		OpenGLGraphicsShader(const OpenGLGraphicsShader &) = delete;
		OpenGLGraphicsShader & operator= (const OpenGLGraphicsShader &) = delete;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void Reload() override;
        virtual void ReloadIfAnyChanges() override;
        inline virtual std::string GetName() const override;
		inline virtual void SetName(const std::string& name) override;
		virtual std::vector<ShaderSourceField> GetSources() const override;
		virtual void SetSources(const std::vector<ShaderSourceField>& sources) override;
    	virtual void SetInt(const std::string& name, int value) override;

    	virtual bool IsForward() const override {return m_IsForward;};
    	virtual void SetForward(bool isForward) override {m_IsForward = isForward;};
    private:
		OpenGLShader m_OpenGLShader;
    	bool m_IsForward = true;
    };

	class OpenGLComputeShader : public ComputeShader
	{
	public:
		OpenGLComputeShader(const std::string& name, ShaderSourceField source);
	public:
		OpenGLComputeShader (const OpenGLComputeShader&) = delete;
		OpenGLComputeShader& operator= (const OpenGLComputeShader&) = delete;

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Reload() override;
        virtual void ReloadIfAnyChanges() override;
	public:
		inline virtual std::string GetName() const override;
		inline virtual void SetName(const std::string& name) override;
	public:
		virtual ShaderSourceField GetSource() const override;
		virtual void SetSource(ShaderSourceField source) override;
	public:
		virtual void Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ) override;
	public:
		OpenGLShader m_OpenGLShader;
	};

} // Core
