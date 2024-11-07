//
// Created by Sayama on 10/10/2024.
//

#pragma once

#include "Voxymore/Core/UUID.hpp"
#include "Voxymore/Renderer/Shader.hpp"

namespace Voxymore::Core
{
	class Action
	{
	public:
		// Using a pair to have a string setting the name as an helper for the research if the ID is invalid.
		using ParameterKey = std::pair<UUID, std::string>;
	public:
		virtual ~Action() = default;
	public:
		virtual bool Valid() const = 0;
		virtual void Execute() = 0;
	public:
		std::vector<ParameterKey> m_Inputs;
		std::vector<ParameterKey> m_Outputs;
	};

	class ComputeShaderAction : public Action
	{
	public:
		ComputeShaderAction(ComputeShaderField computeShader, uint32_t groupX, uint32_t groupY, uint32_t groupZ);
		virtual ~ComputeShaderAction();
	public:
		virtual bool Valid() const override;
		virtual void Execute() override;
	private:
		uint32_t groupX, groupY, groupZ;
		ComputeShaderField m_ComputeShader;
	};
} // namespace Voxymore::Core

