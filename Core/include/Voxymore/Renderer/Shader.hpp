//
// Created by ianpo on 20/07/2023.
//

#pragma once

#include "Voxymore/Core/SmartPointers.hpp"
#include "Voxymore/Assets/Asset.hpp"
#include "Voxymore/Assets/AssetField.hpp"
#include "Voxymore/Renderer/ShaderSource.hpp"
#include "Voxymore/Renderer/ShaderType.hpp"
#include <unordered_map>

namespace Voxymore::Core {


    class Shader : public Asset
	{
    public:
		VXM_IMPLEMENT_ASSET(AssetType::Shader);
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
		virtual void Reload() = 0;
		virtual void ReloadIfAnyChanges() = 0;

        virtual std::string GetName() const = 0;
        virtual void SetName(const std::string& name) = 0;

		virtual std::vector<ShaderSourceField> GetSources() const = 0;
		virtual void SetSources(const std::vector<ShaderSourceField>& sources) = 0;

    	virtual bool IsForward() const = 0;
    	virtual void SetForward(bool isForward) = 0;

    	[[deprecated]] virtual void SetInt(const std::string& name, int value) = 0;

		static Ref<Shader> Create(const std::string& name, const std::unordered_map<ShaderType, ShaderSourceField>& sources);
		static Ref<Shader> Create(const std::string& name, const std::vector<ShaderSourceField>& sources);
    };

	class ComputeShader : public Asset
	{
	public:
		VXM_IMPLEMENT_ASSET(AssetType::ComputeShader);
		virtual ~ComputeShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Reload() = 0;
		virtual void ReloadIfAnyChanges() = 0;
	public:
		virtual std::string GetName() const = 0;
		virtual void SetName(const std::string& name) = 0;
	public:
		virtual void SetSource(ShaderSourceField source) = 0;
		virtual ShaderSourceField GetSource() const = 0;
	public:
		virtual void Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ) = 0;
	public:
		static Ref<ComputeShader> Create(const std::string& name, ShaderSourceField source);
	};

	using ShaderField = AssetField<Shader>;
	using ComputeShaderField = AssetField<ComputeShader>;

} // Core
