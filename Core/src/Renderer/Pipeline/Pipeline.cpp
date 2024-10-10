//
// Created by Sayama on 10/10/2024.
//

#include "Voxymore/Renderer/Pipeline/Pipeline.hpp"


namespace Voxymore::Core
{
	Core::Pipeline::Pipeline() = default;

	Pipeline::~Pipeline() = default;

	Pipeline::Pipeline(Pipeline &&other) noexcept
			: m_Parameters(std::move(other.m_Parameters)), m_Actions(std::move(other.m_Actions))
	{
	}

	Pipeline &Pipeline::operator=(Pipeline &&other) noexcept
	{
		swap(other);
		return *this;
	}

	void Pipeline::swap(Pipeline &other) noexcept
	{
		std::swap(m_Parameters, other.m_Parameters);
		std::swap(m_Actions, other.m_Actions);
	}

	void Pipeline::Execute(Action& action)
	{
		std::unordered_set<UUID> params;
		params.reserve(action.m_Inputs.size() + action.m_Outputs.size());

		for (auto& key : action.m_Inputs)
		{
			auto param = FindParameter(key);
			if(!param) continue;

			if(params.contains(param.value().first)) continue;

			//TODO: do something with the params. I don't know.

			params.emplace(param.value().first);
		}

		for (auto& key : action.m_Outputs)
		{
			auto param = FindParameter(key);
			if(!param) continue;

			if(params.contains(param.value().first)) continue;

			//TODO: do something with the params. I don't know.

			params.emplace(param.value().first);
		}

		action.Execute();
	}

	std::optional<std::pair<UUID, Pipeline::RendererParameter&>> Pipeline::FindParameter(const Action::ParameterKey &parameterKey)
	{
		const auto& [id, name] = parameterKey;

		auto it = m_Parameters.find(id);
		if(it != m_Parameters.end()) {
			return {{it->first, it->second}};
		}

		it = std::find_if(m_Parameters.begin(), m_Parameters.end(), [&name](const std::pair<UUID, Pipeline::RendererParameter>& params) {
		  return params.second.m_Parameter.m_Name.find(name) != std::string::npos;
		});

		if(it != m_Parameters.end()) {
			return {{it->first, it->second}};
		}

		return std::nullopt;
	}
} // namespace Voxymore::Core
