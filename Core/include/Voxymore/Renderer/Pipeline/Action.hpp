//
// Created by Sayama on 10/10/2024.
//

#pragma once

#include "Voxymore/Core/UUID.hpp"

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
		virtual void Execute() = 0;
	public:
		std::vector<ParameterKey> m_Inputs;
		std::vector<ParameterKey> m_Outputs;
	};
} // namespace Voxymore::Core

