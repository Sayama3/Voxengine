//
// Created by ianpo on 05/01/2024.
//

#pragma once

#include "Voxymore/Scene/Systems.hpp"

namespace Voxymore::Core
{

	class ParticleDragSystem : public System
	{
		VXM_IMPLEMENT_SYSTEM(ParticleDragSystem);
	protected:
		virtual void DeserializeSystem(YAML::Node& node) override;
		virtual void SerializeSystem(YAML::Emitter& out) override;
		virtual void ResetSystem() override;
	public:
		virtual bool OnImGuiRender() override;
		virtual void Update(Scene& scene, TimeStep ts) override;

		inline virtual bool RunOnAllScenes() override { return true; }
	private:
		Real m_DragCoef = 0.9;
		Real m_DragCoefSqr = 0.8;
	};

} // namespace Voxymore::Core

