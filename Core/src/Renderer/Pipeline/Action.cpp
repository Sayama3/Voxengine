//
// Created by Sayama on 10/10/2024.
//

#include "Voxymore/Renderer/Pipeline/Action.hpp"

namespace Voxymore
{
	namespace Core
	{
		ComputeShaderAction::ComputeShaderAction(ComputeShaderField computeShader, uint32_t groupX, uint32_t groupY, uint32_t groupZ) : m_ComputeShader(computeShader), groupX(groupX), groupY(groupY), groupZ(groupZ)
		{
		}

		ComputeShaderAction::~ComputeShaderAction()
		{
		}

		bool ComputeShaderAction::Valid() const
		{
			return m_ComputeShader.IsValid();
		}

		void ComputeShaderAction::Execute()
		{
			m_ComputeShader.GetAsset()->Dispatch(groupX, groupY, groupZ);
		}
	}// namespace Core
}// namespace Voxymore